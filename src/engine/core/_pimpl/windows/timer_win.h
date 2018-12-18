#pragma once

namespace sb {

Timer::Ctx Timer::getCtx()
{
    Ctx ctx;
    QueryPerformanceFrequency(&ctx);

    return ctx;
}

Timer::Value Timer::now()
{
    LARGE_INTEGER val;
    QueryPerformanceCounter(&val);

    return val.QuadPart;
}

ui64 Timer::toMs(Ctx const & ctx, Value val)
{
    return (ui64)((val * 1000.0) / (f64)ctx.QuadPart);
}

f64 Timer::toSec(Ctx const & ctx, Value val)
{
    return val / (f64)ctx.QuadPart;
}

}
