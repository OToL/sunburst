#pragma once

#include <sb_core/core.h>

#include <sb_std/span>
#include <sb_std/string_view>

namespace sb {

u32 computeCRC32(sbstd::string_view buffer);
u32 computeCRC32(sbstd::span<u8 const> buffer);
u32 computeCRC32(char const * const str);

u64 computeCRC64(sbstd::string_view buffer);
u64 computeCRC64(sbstd::span<u8 const> buffer);
u64 computeCRC64(char const * const str);

} // namespace sb
