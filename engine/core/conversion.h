#pragma once

#include "_impl/conversion.h"

#include <wstd/span>
#include <wstd/type_traits>
#include <wstd/utility>

namespace sb {

template <typename TDst, typename TSrc>
inline constexpr TDst numericConv(TSrc && src)
{
    return static_cast<TDst>(src);
}

// Ensures null termination
template <typename TSrc>
inline usize stringConvT(TSrc && src, wstd::span<char> dest)
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
