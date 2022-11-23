#pragma once

#include <sb_core/core.h>

#include <sb_slw/forward_decls>

namespace sb {

inline constexpr u32 computeFNV1a32(char const * const str);
inline constexpr u32 computeFNV1a32(char const * data, size_t len);
inline constexpr u32 computeFNV1a32(u8 const * data, size_t len);
u32 computeFNV1a32(slw::string_view_fw const & buffer);
u32 computeFNV1a32(slw::span_fw<u8 const> const & buffer);

inline constexpr u64 computeFNV1a64(char const * const str);
inline constexpr u64 computeFNV1a64(char const * data, size_t len);
inline constexpr u64 computeFNV1a64(u8 const * data, size_t len);
u64 computeFNV1a64(slw::string_view_fw const & buffer);
u64 computeFNV1a64(slw::span_fw<u8 const> const & buffer);

} // namespace sb

#include <sb_core/_impl/hash/fnv1a.h>
