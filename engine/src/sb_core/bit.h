#pragma once

#include <sb_core/core.h>
#include <sb_slw/bit>

namespace sb {

template <typename T>
constexpr i32 countLeadingZero(T val)
{
    return slw::countl_zero(val);
}

} // namespace sb
