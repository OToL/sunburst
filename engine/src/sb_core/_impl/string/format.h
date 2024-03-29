#pragma once

#include <sb_core/cast.h>
#include <sb_core/string/conversion.h>

#include <sb_slw/utility>
#include <sb_slw/span>

namespace sb::internal {

template <typename T>
inline usize fmtArgFwCall(void const * value, slw::span_fw<char> dest_buffer)
{
    return fmtArg(*reinterpret_cast<T *>(value), dest_buffer);
}

using FmtFwCallCB = usize (*)(void const *, slw::span_fw<char> const &);

struct FmtArg
{
    void const * value;
    FmtFwCallCB fmt_cb;
};

inline void expandFmtArgs(slw::span_fw<FmtArg> const &) { }

template <typename T, typename = void>
struct FmtArgDesc
{
    using ValueType = T;
};

template <typename T>
struct FmtArgDesc<T, slw::enable_if_t<slwx::is_string_raw<slw::decay_t<T>>::value>>
{
    using ValueType = char const *;

    inline static void const * storeValue(ValueType value)
    {
        return reinterpret_cast<void const *>(value);
    }

    inline static ValueType extractValue(void const * value)
    {
        return reinterpret_cast<ValueType>(value);
    }
};

template <typename T>
struct FmtArgDesc<T, slw::enable_if_t<slw::is_arithmetic<T>::value>>
{
    using ValueType = slw::remove_cv_t<slw::remove_reference_t<T>>;

    inline static void const * storeValue(ValueType const & value)
    {
        return reinterpret_cast<void const *>(&value);
    }

    inline static ValueType const & extractValue(void const * value)
    {
        return *reinterpret_cast<ValueType const *>(value);
    }
};

template <typename T, typename... TArgs>
inline void expandFmtArgs(slw::span<FmtArg> argList, T const & arg, TArgs &&... args)
{
    FmtArg & arg_desc = argList[0];

    using TypeDesc = FmtArgDesc<T>;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    arg_desc.value = TypeDesc::storeValue(arg);
    arg_desc.fmt_cb = [](void const * arg_value, slw::span<char> const &dest) {
        return toString(TypeDesc::extractValue(arg_value), dest);
    };

    expandFmtArgs(argList.subspan(1), args...);
}

usize formatString(slw::span<char> dest_buffer, char const * const format, slw::span<FmtArg> agrs);

template <typename... TArgs>
inline usize formatString(char * dest_buffer, usize capacity, char const * const format, TArgs &&... args)
{
    return sb::formatString({dest_buffer, capacity}, format, slw::forward<TArgs>(args)...);
}

} // namespace sb::internal

template <typename... TArgs>
inline sb::usize sb::formatString(slw::span<char> const &dest_buffer, char const * const format, TArgs &&... args)
{
    internal::FmtArg arg_list[sizeof...(TArgs)];
    expandFmtArgs(arg_list, args...);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    return internal::formatString(dest_buffer, format, {arg_list, sizeof...(TArgs)});
}

namespace sb {

inline usize formatString(slw::span<char> dest_buffer, char const * const format)
{
    return internal::formatString(dest_buffer, format, {});
}

} // namespace sb

