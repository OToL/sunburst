#pragma once

#include <sb_std/cstddef>
#include <sb_std/cstdint>

namespace sb {

using u8 = uint8_t;
using s8 = int8_t;
using s16 = int16_t;
using u16 = uint16_t;
using s32 = int32_t;
using u32 = uint32_t;
using u64 = uint64_t;
using s64 = int64_t;
using uptr = uintptr_t;
using sptrdiff = ptrdiff_t;
using usize = size_t;
using f32 = float;
using f64 = double;
using b8 = bool;
using b32 = u32;

} // namespace sb

#define sbBaseClass(...) using BaseClass = __VA_ARGS__

#define SB_DISABLED 1
#define SB_ENABLED 2

#define sbIsEnabled(feature) (1 == 2 / SB_CTF_##feature)
#define sbIsEqual(feature, value) ((1 / SB_CTV_##feature + 1) && (SB_CTV_##feature == (value)))
