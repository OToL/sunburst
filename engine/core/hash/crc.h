#pragma once

#include <core/types.h>

namespace sb {

struct CRC32
{
    using Value = ui32;

    static constexpr Value NULL_VALUE = 0U;

    static Value compute(ui8 const * const buffer, usize const len);
    static Value compute(char const * const str);
};

struct CRC64
{
    using Value = ui64;

    static constexpr Value NULL_VALUE = 0ULL;

    static Value compute(ui8 const * const buffer, usize const len);
    static Value compute(char const * const str);
};

} // namespace sb
