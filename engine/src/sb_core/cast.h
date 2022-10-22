#pragma once

#include <sb_core/core.h>
#include <sb_core/compiler.h>

#include <sb_slw/type_traits>
#include <sb_slw/utility>

// Those functions follow stl cast naming convention for consistency
namespace sb {

// @todo: implement value limit check
template <typename TDst, typename TSrc>
inline constexpr TDst integral_cast(TSrc src)
{
    return static_cast<TDst>(src);
}

template <typename TSrc, typename TDst = slw::underlying_type_t<TSrc>>
inline constexpr auto integral_cast(TSrc src) -> slw::enable_if_t<slw::is_enum_v<TSrc>, slw::underlying_type_t<TSrc>>
{
    if constexpr (slw::is_same_v<TDst, slw::underlying_type_t<TSrc>>)
    {
        return static_cast<slw::underlying_type_t<TSrc>>(src);
    }
    else
    {
        return integral_cast<TDst>(static_cast<slw::underlying_type_t<TSrc>>(src));
    }
}

template <typename TDst, typename TSrc>
inline constexpr TDst integral_trunc_cast(TSrc src)
{
    return static_cast<TDst>(src);
}

} // namespace sb
