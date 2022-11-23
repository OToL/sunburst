#include <sb_core/profiling_system.h>
#include <sb_core/error.h>

#include <sb_core/extern/windows/windows.h>

static_assert(sizeof(sb::profiling_system::PerfCounter) == sizeof(LARGE_INTEGER));
static_assert(alignof(sb::profiling_system::PerfCounter) == alignof(LARGE_INTEGER));

static LARGE_INTEGER g_hwfrequency = {};
static sb::f64 g_hwfrequency_ms = {};
static sb::b8 g_isInitialized = false;

sb::b8 sb::profiling_system::isInitialized()
{
   return g_isInitialized; 
}

sb::b8 sb::profiling_system::initialize()
{
    sb_assert((g_hwfrequency.LowPart == 0) && (g_hwfrequency.HighPart == 0));
    sb_assert(!g_isInitialized);

    QueryPerformanceFrequency(&g_hwfrequency);
    g_hwfrequency_ms = ((f64)g_hwfrequency.QuadPart)/1000.0;
    g_isInitialized = true;

    return true;
}

sb::b8 sb::profiling_system::terminate()
{
    sb_assert(g_isInitialized);
    g_hwfrequency = {};
    g_isInitialized = false;
    return true;
}

sb::b8 sb::profiling_system::getPerfCounter(PerfCounter &val)
{
    return QueryPerformanceCounter((LARGE_INTEGER *)&val);
}

sb::f32 sb::profiling_system::toMilliSeconds(PerfCounter val)
{
    return (f32)((f64)val.quad_part/g_hwfrequency_ms);
}

sb::f32 sb::profiling_system::toSeconds(PerfCounter val)
{
    return (f32)((f64)val.quad_part/(f64)g_hwfrequency.QuadPart);
}
