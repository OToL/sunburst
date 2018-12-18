#pragma once

#include "_impl/conversion.h"

#include <libc++/span>
#include <libc++/type_traits>
#include <libc++/utility>

namespace sb {

template <typename TDst, typename TSrc>
constexpr TDst numericCast(TSrc && src)
{
    return static_cast<TDst>(src);
}

// Ensures null termination
template <typename TSrc>
usize stringCastT (TSrc && src, wstd::span<char> dest)
{
    using DecayedType = wstd::decay_t<TSrc>;

    if constexpr (wstdx::is_string_raw<DecayedType>::value)
    {
        return detail::stringToCharBuffer(src, dest);
    }
    else if constexpr (wstd::is_integral<DecayedType>::value)
    {
        return detail::decimalToString(wstd::forward<TSrc>(src), dest);
    }
    else
    {
        static_assert(wstdx::unreachable_v<TSrc>, "Unhandled string conversion");
    }
}

} // namespace sb
