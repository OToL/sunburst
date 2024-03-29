#pragma once

#include <sb_core/core.h>

namespace sb {

struct MemoryTag
{
    char const * value;
};

constexpr MemoryTag operator"" _mt(char const * str, usize /*len*/)
{
    return {str};
}

} // namespace sb
