#pragma once

#include "core.h"

#if defined(SB_PLATFORM_WINDOWS)
#    include "_impl/profiling_win.h"
#else
#    error "Unsupported platform"
#endif

namespace sb::profiling {

using sb::internal::PerfCounter;

b8 initialize();

b8 terminate();

b8 getPerfCounter(PerfCounter &val);

f32 toMilliSeconds(PerfCounter val);

f32 toSeconds(PerfCounter val);

}

