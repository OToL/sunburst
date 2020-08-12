#pragma once

#include <base/base.h> 

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
        return internal::stringToCharBuffer(src, dest);
    }
    else if constexpr (wstd::is_integral<DecayedType>::value)
    {
        return internal::decimalToString(wstd::forward<TSrc>(src), dest);
    }
    else
    {
        static_assert(false, "Unhandled string conversion");
    }
}

} // namespace sb

#include "_impl/conversion.h"
