#pragma once

#include <sb_core/core.h>
#include <sb_core/compiler.h>

#include <sb_std/type_traits>
#include <sb_std/utility>

// Those functions follow stl cast naming convention for consistency
namespace sb {

// @todo: implement value limit check
template <typename TDst, typename TSrc>
inline constexpr TDst integral_cast(TSrc src)
{
    return static_cast<TDst>(src);
}

template <typename TSrc, typename TDst = sbstd::underlying_type_t<TSrc>>
inline constexpr auto integral_cast(TSrc src) -> sbstd::enable_if_t<sbstd::is_enum_v<TSrc>, sbstd::underlying_type_t<TSrc>>
{
    if constexpr (sbstd::is_same_v<TDst, sbstd::underlying_type_t<TSrc>>)
    {
        return static_cast<sbstd::underlying_type_t<TSrc>>(src);
    }
    else
    {
        return integral_cast<TDst>(static_cast<sbstd::underlying_type_t<TSrc>>(src));
    }
}

template <typename TDst, typename TSrc>
inline constexpr TDst integral_trunc_cast(TSrc src)
{
    return static_cast<TDst>(src);
}

} // namespace sb
