#pragma once

#include <sb_core/core.h>
#include <sb_core/compiler.h>
#include <sb_core/_impl/string/conversion.h>

#include <sb_slw/type_traits>
#include <sb_slw/utility>

namespace sb {

// Ensures null termination
template <typename TSrc>
inline usize toString(TSrc && src, slw::span<char> dest) requires slw::is_integral<slw::decay_t<TSrc>>::value || slwx::is_string_raw<slw::decay_t<TSrc>>::value
{
    using DecayedType = slw::decay_t<TSrc>;

    if constexpr (slwx::is_string_raw<DecayedType>::value)
    {
        return internal::stringToCharBuffer(&src[0], dest);
    }
    else if constexpr (slw::is_integral<DecayedType>::value)
    {
        return internal::decimalToString(slw::forward<TSrc>(src), dest);
    }
    else
    {
        return 0U;
    }
}

} // namespace sb
