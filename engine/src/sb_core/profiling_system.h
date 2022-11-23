#pragma once

#include "core.h"

#if defined(SB_PLATFORM_WINDOWS)
#    include "_impl/profiling_system_win.h"
#else
#    error "Unsupported platform"
#endif

namespace sb {

namespace profiling_system {

    using sb::internal::PerfCounter;

    b8 initialize();

    b8 isInitialized();

    b8 terminate();

    b8 getPerfCounter(PerfCounter & val);

    f32 toMilliSeconds(PerfCounter val);

    f32 toSeconds(PerfCounter val);

} // namespace profiling_system

namespace profs = profiling_system;

} // namespace sb
