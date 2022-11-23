#pragma once

#include <sb_core/core.h>

namespace sb::internal {

using PerfCounter = union  {
    struct {
        u32 low_part;
        i32 high_part;
    };
    i64 quad_part;
};

inline PerfCounter operator - (PerfCounter &lval, PerfCounter &rval)
{
    return {.quad_part = lval.quad_part - rval.quad_part};
}

}

