#include <sb_core/profiling.h>
#include <sb_core/error.h>

#include <sb_core/extern/windows/windows.h>

static_assert(sizeof(sb::profiling::PerfCounter) == sizeof(LARGE_INTEGER));
static_assert(alignof(sb::profiling::PerfCounter) == alignof(LARGE_INTEGER));

static LARGE_INTEGER g_hwfrequency = {};
static sb::f64 g_hwfrequency_ms = {};

sb::b8 sb::profiling::initialize()
{
    sb_assert((g_hwfrequency.LowPart == 0) && (g_hwfrequency.HighPart == 0));

    QueryPerformanceFrequency(&g_hwfrequency);
    g_hwfrequency_ms = ((f64)g_hwfrequency.QuadPart)/1000.0;

    return true;
}

sb::b8 sb::profiling::terminate()
{
    g_hwfrequency = {};
    return true;
}

sb::b8 sb::profiling::getPerfCounter(PerfCounter &val)
{
    return QueryPerformanceCounter((LARGE_INTEGER *)&val);
}

sb::f32 sb::profiling::toMilliSeconds(PerfCounter val)
{
    return (f32)((f64)val.quad_part/g_hwfrequency_ms);
}

sb::f32 sb::profiling::toSeconds(PerfCounter val)
{
    return (f32)((f64)val.quad_part/(f64)g_hwfrequency.QuadPart);
}
