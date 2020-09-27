#pragma once

#include <sb_core/conversion.h>

#include <sb_std/utility>

namespace sb::internal {

template <typename T>
inline usize fmtArgFwCall(void const * value, sbstd::span<char> dest_buffer)
{
    return fmtArg(*reinterpret_cast<T *>(value), dest_buffer);
}

typedef usize (*FmtFwCallCB)(void const *, sbstd::span<char>);

struct FmtArg
{
    void const * m_value;
    FmtFwCallCB m_fmt_cb;
};

inline void expandFmtArgs(sbstd::span<FmtArg>) { }

template <typename T, typename = void>
struct FmtArgDesc
{
    using ValueType = T;
};

template <typename T>
struct FmtArgDesc<T, sbstd::enable_if_t<sbstdx::is_string_raw<sbstd::decay_t<T>>::value>>
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
struct FmtArgDesc<T, sbstd::enable_if_t<sbstd::is_arithmetic<T>::value>>
{
    using ValueType = sbstd::remove_cv_t<sbstd::remove_reference_t<T>>;

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
inline void expandFmtArgs(sbstd::span<FmtArg> argList, T const & arg, TArgs &&... args)
{
    FmtArg & arg_desc = argList[0];

    using TypeDesc = FmtArgDesc<T>;

    arg_desc.m_value = TypeDesc::storeValue(arg);
    arg_desc.m_fmt_cb = [](void const * arg_value, sbstd::span<char> dest) {
        return stringConvT(TypeDesc::extractValue(arg_value), dest);
    };

    expandFmtArgs(argList.subspan(1), args...);
}

usize stringFormat(sbstd::span<char> dest_buffer, char const * const format,
                    sbstd::span<FmtArg> agrs);

template <typename... TArgs>
inline usize stringFormat(char * dest_buffer, usize capacity, char const * const format,
                            TArgs &&... args)
{
    return sb::stringFormat({dest_buffer, (sptrdiff)capacity}, format,
                            sbstd::forward<TArgs>(args)...);
}

}

template <typename... TArgs>
inline sb::usize sb::stringFormat(sbstd::span<char> dest_buffer, char const * const format,
                          TArgs &&... args)
{
    internal::FmtArg arg_list[sizeof...(TArgs)];
    expandFmtArgs(arg_list, args...);

    return internal::stringFormat(dest_buffer, format, {arg_list, sizeof...(TArgs)});
}


namespace sb{

inline usize stringFormat(sbstd::span<char> dest_buffer, char const * const format)
{
    return internal::stringFormat(dest_buffer, format, {});
}

}