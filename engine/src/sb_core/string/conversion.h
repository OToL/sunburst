#pragma once

#include <sb_core/core.h>
#include <sb_core/compiler.h>
#include <sb_core/_impl/string/conversion.h>

#include <sb_std/type_traits>
#include <sb_std/utility>

namespace sb {

// Ensures null termination
template <typename TSrc>
inline usize toString(TSrc && src, sbstd::span<char> dest) requires sbstd::is_integral<sbstd::decay_t<TSrc>>::value || sbstdx::is_string_raw<sbstd::decay_t<TSrc>>::value
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
        return 0U;
    }
}

} // namespace sb
