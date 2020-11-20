#pragma once

#include <sb_core/core.h>
#include <sb_core/compiler.h>

#include <sb_std/span>
#include <sb_std/type_traits>
#include <sb_std/utility>

#include "_impl/conversion.h"

namespace sb {

template <typename TDst, typename TSrc>
inline constexpr TDst numericConv(TSrc && src)
{
    return static_cast<TDst>(src);
}

// Ensures null termination
template <typename TSrc>
inline usize stringConvT(TSrc && src, sbstd::span<char> dest)
{
    using DecayedType = sbstd::decay_t<TSrc>;

    if constexpr (sbstdx::is_string_raw<DecayedType>::value)
    {
        return internal::stringToCharBuffer(&src[0], dest);
    }
    else if constexpr (sbstd::is_integral<DecayedType>::value)
    {
        return internal::decimalToString(sbstd::forward<TSrc>(src), dest);
    }
    else
    {
        sbStaticUnreachable("Unsupported conversion");
        return 0U;
    }
}

} // namespace sb
