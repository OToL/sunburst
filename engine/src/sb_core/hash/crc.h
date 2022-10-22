#pragma once

#include <sb_core/core.h>

#include <sb_slw/span>
#include <sb_slw/string_view>

namespace sb {

u32 computeCRC32(slw::string_view buffer);
u32 computeCRC32(slw::span<u8 const> buffer);
u32 computeCRC32(char const * const str);

u64 computeCRC64(slw::string_view buffer);
u64 computeCRC64(slw::span<u8 const> buffer);
u64 computeCRC64(char const * const str);

} // namespace sb
