#pragma once

#include <libc++/cstddef>
#include <libc++/cstdint>

#if defined(SB_TARGET_MACOS)
#    include <TargetConditionals.h>
#    if TARGET_OS_MAC
#        include "_pimpl/macos/platform_macos.h"
#    else
#        error "Unhandled Apple platform"
#    endif
#elif defined(SB_TARGET_LINUX)
#    include "_pimpl/linux/platform_linux.h"
#else
#    error "Unhandled target platform"
#endif

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

constexpr usize GLOBAL_HEAP_MIN_ALIGNMENT = 8;
constexpr char const * PHYSICAL_PATH_SEPARATOR = detail::PHYSICAL_PATH_SEPARATOR;
constexpr usize PHYSICAL_PATH_MAX_LEN = detail::PHYSICAL_PATH_MAX_LEN;

char const * getWorkingDirectory();
void outputDebugString(char const * msg);

} // namespace sb
