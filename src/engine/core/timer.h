#pragma once

#include "platform.h"

#if defined(SB_TARGET_MACOS)
#   include "_pimpl/macos/timer_macos.h"
#else
#   error "Unhandled target platform"
#endif

namespace sb {

struct Timer : public detail::Timer
{
    sbConstructProtect(Timer);
    sbBaseClass(detail::Timer);

    using detail::Ctx;
    using detail::Value;

    using BaseClass::getCtx;
    using BaseClass::now;
    using BaseClass::toMs;
    using BaseClass::toSec;
};

} // namespace sb
