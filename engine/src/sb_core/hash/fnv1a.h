#pragma once

#include <sb_core/core.h>

#include <sb_slw/span>
#include <sb_slw/string_view>

namespace sb {

inline constexpr u32 computeFNV1a32(slw::string_view buffer);
inline constexpr u32 computeFNV1a32(slw::span<u8 const> buffer);
inline constexpr u32 computeFNV1a32(char const * const str);

inline constexpr u64 computeFNV1a64(slw::string_view buffer);
inline constexpr u64 computeFNV1a64(slw::span<u8 const> buffer);
inline constexpr u64 computeFNV1a64(char const * const str);

} // namespace sb

#include <sb_core/_impl/hash/fnv1a.h>
