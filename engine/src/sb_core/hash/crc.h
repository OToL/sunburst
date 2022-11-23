#pragma once

#include <sb_core/core.h>

#include <sb_slw/forward_decls>

namespace sb {

u32 computeCRC32(slw::string_view_fw const &buffer);
u32 computeCRC32(slw::span_fw<u8 const> const &buffer);
u32 computeCRC32(char const * const str);

u64 computeCRC64(slw::string_view_fw const &buffer);
u64 computeCRC64(slw::span_fw<u8 const> const &buffer);
u64 computeCRC64(char const * const str);

} // namespace sb
