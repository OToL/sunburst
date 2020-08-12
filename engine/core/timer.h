#pragma once

#if 0

#include <base/base.h>

#if defined(SB_PLATFORM_WINDOWS)
#    include "_impl/platform/timer_decl_win.h"
#else
#    error "Unhandled target platform"
#endif

namespace sb {

struct Timer
{
    sbConstructProtect(Timer);

    using Ctx = detail::TimerCtx;
    using Value = detail::TimerValue;

    static inline Ctx getCtx();

    static inline Value now();

    static inline ui64 toMs(Ctx const & ctx, Value val);

    static inline f64 toSec(Ctx const & ctx, Value val);
};

} // namespace sb

#if defined(SB_PLATFORM_WINDOWS)
#    include "_impl/platform/timer_win.h"
#else
#    error "Unhandled target platform"
#endif

#endif
