#pragma once

#include <core/container/span.h>
#include "_impl/conversion.h"

#include <libc++/type_traits>

namespace sb {

template <typename TDst, typename TSrc>
constexpr TDst numericCast(TSrc && src)
{
    return static_cast<TDst>(src);
}

template <typename TSrc>
usize stringCast(TSrc && src, sb::Span<char> dest)
{
    using DecayedType = wstd::decay_t<TSrc>;

    if constexpr (wstdx::is_string_raw<DecayedType>::value)
    {
        return detail::stringToCharBuffer(src, dest);
    }
    else if constexpr (wstd::is_integral<DecayedType>::value && !wstd::is_same<DecayedType, bool>::value && (sizeof(DecayedType) <= 8))
    {
        return detail::decimalToString(src, dest);
    }
    else
    {
        static_assert(wstdx::unreachable_v<TSrc>, "Unhandled string conversion");
    }
}

} // namespace sb
