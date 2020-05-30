#pragma once

#include <core/types.h>

namespace sb {

// returns number of characters copied without the terminating '\0'
usize strCpyT(char * sb_restrict output, usize capacity, char const * sb_restrict src);

// returns number of characters copied without the terminating '\0'
template <usize COUNT>
inline usize strCpyT(char (&output)[COUNT], char const * sb_restrict src)
{
    return strCpyT(&output[0], COUNT, src);
}

// returns true if 'str' begins with 'start_str'
bool strStartWith(char const * sb_restrict str, char const * sb_restrict start_str);

} // namespace sb
