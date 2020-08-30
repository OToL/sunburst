#pragma once

#include <sb_base/base.h>

#include <sb_std/span>
#include <sb_std/string_view>

namespace sb {

ui32 computeCRC32(sbstd::string_view buffer);
ui32 computeCRC32(sbstd::span<ui8 const> buffer);
ui32 computeCRC32(char const * const str);

ui64 computeCRC64(sbstd::string_view buffer);
ui64 computeCRC64(sbstd::span<ui8 const> buffer);
ui64 computeCRC64(char const * const str);

} // namespace sb
