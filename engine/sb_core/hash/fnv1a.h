#pragma once

#include <sb_base/base.h>

namespace sb {

inline constexpr ui32 computeFNV1a32(ui8 const * const buffer, usize const len);
inline constexpr ui32 computeFNV1a32(char const * const str);

inline constexpr ui64 computeFNV1a64(ui8 const * const buffer, usize const len);
inline constexpr ui64 computeFNV1a64(char const * const str);

} // namespace sb

#include <sb_core/_impl/hash/fnv1a.h>
