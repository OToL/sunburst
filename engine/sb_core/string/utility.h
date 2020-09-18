#pragma once

#include <sb_core/core.h>

namespace sb {

// returns number of characters copied without the terminating '\0'
usize strCpyT(char * __restrict output, usize capacity, char const * __restrict src);

// returns number of characters copied without the terminating '\0'
template <usize COUNT>
inline usize strCpyT(char (&output)[COUNT], char const * __restrict src)
{
    return strCpyT(&output[0], COUNT, src);
}

// returns true if 'str' begins with 'start_str'
b8 strStartWith(char const * __restrict str, char const * __restrict start_str);

} // namespace sb
