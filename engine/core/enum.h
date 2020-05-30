#pragma once

#include <libc++/type_traits>

namespace sb {

template <typename TEnum>
inline constexpr auto getEnumValue(TEnum val)
{
    return (wstd::underlying_type_t<TEnum>)val;
}

template <typename TEnum, typename... TEnums>
inline constexpr auto makeEnumMask(TEnum val, TEnums... others)
{
    return (wstd::underlying_type_t<TEnum>)(getEnumValue(others) | ... | getEnumValue(val));
}

} // namespace sb
