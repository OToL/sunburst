#pragma once

#include <sb_base/base.h>

#include <sb_std/span>
#include <sb_std/string_view>

namespace sb {

inline constexpr ui32 computeFNV1a32(sbstd::string_view buffer);
inline constexpr ui32 computeFNV1a32(sbstd::span<ui8 const> buffer);
inline constexpr ui32 computeFNV1a32(char const * const str);

inline constexpr ui64 computeFNV1a64(sbstd::string_view buffer);
inline constexpr ui64 computeFNV1a64(sbstd::span<ui8 const> buffer);
inline constexpr ui64 computeFNV1a64(char const * const str);

} // namespace sb

#include <sb_core/_impl/hash/fnv1a.h>
