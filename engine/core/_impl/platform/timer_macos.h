#pragma once

#include <mach/mach_time.h>

namespace sb::detail {

class Timer
{
public:

    using Ctx = mach_timebase_info_data_t;
    using Value = ui64;

    static inline void getCtx(Ctx & ctx)
    {
        mach_timebase_info(&ctx);
    }

    static inline Value now()
    {
        return mach_absolute_time();
    }

    static inline ui64 toMs(Ctx const & ctx, Value val)
    {
        return (val * ctx.numer / ctx.denom) / 1000000ULL;
    }

    static inline f64 toSec(Ctx const & ctx, Value val)
    {
        return ((f64)(val * ctx.numer / ctx.denom)) / NSEC_PER_SEC;
    }
};

}
