#pragma once

#include <sb_base/base.h>

namespace sb {

ui32 computeCRC32(ui8 const * const buffer, usize const len);
ui32 computeCRC32(char const * const str);

ui64 computeCRC64(ui8 const * const buffer, usize const len);
ui64 computeCRC64(char const * const str);

} // namespace sb
