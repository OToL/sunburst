#pragma once

#include <wstd/cstddef>
#include <wstd/cstdint>

#include "compiler.h"

namespace sb {

using ui8 = uint8_t;
using si8 = int8_t;
using si16 = int16_t;
using ui16 = uint16_t;
using ui64 = uint64_t;
using si64 = int64_t;
using si32 = int32_t;
using ui32 = uint32_t;
using uiptr = uintptr_t;
using sptrdiff = ptrdiff_t;
using usize = size_t;
using f32 = float;
using f64 = double;
using b8 = bool;
using b32 = ui32;

using TimerValue = si64;

} // namespace sb
