#pragma once

#include <sb_std/type_traits>

namespace sb {

template <typename TEnum>
inline constexpr auto getEnumValue(TEnum val)
{
    return (sbstd::underlying_type_t<TEnum>)val;
}

template <typename TEnum>
struct EnumMask
{
    using ValueType = sbstd::underlying_type_t<TEnum>;
    static_assert(sbstd::is_unsigned_v<ValueType>);

    ValueType value;
};

template <typename TEnum, typename... TEnums>
requires sbstdx::are_same_v<TEnum, TEnums...> inline constexpr auto makeEnumMaskValue(TEnum val, TEnums... others)
{
    using ValueType = sbstd::underlying_type_t<TEnum>;
    return (ValueType)(((((ValueType)1) << getEnumValue(val)) | ... | (((ValueType)1) << getEnumValue(others))));
}

template <typename TEnum, typename... TEnums>
requires sbstdx::are_same_v<TEnum, TEnums...> inline constexpr EnumMask<TEnum> makeEnumMask(TEnum val, TEnums... others)
{
    return EnumMask<TEnum>{makeEnumMaskValue(val, others...)};
}

template <typename TEnum, typename... TEnums>
requires sbstdx::are_same_v<TEnum, TEnums...> inline constexpr bool hasEnumValues(EnumMask<TEnum> enum_mask,
                                                                                  TEnums... values)
{
    typename EnumMask<TEnum>::ValueType const mask_value = makeEnumMaskValue(values...);
    return mask_value == (enum_mask.value & mask_value);
}

template <typename TEnum, typename... TEnums>
requires sbstdx::are_same_v<TEnum, TEnums...> inline constexpr bool hasAnyEnumValue(EnumMask<TEnum> enum_mask,
                                                                                    TEnums... values)
{
    return 0 != (enum_mask.value & makeEnumMaskValue(values...));
}

} // namespace sb
