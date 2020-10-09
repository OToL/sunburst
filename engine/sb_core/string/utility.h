#pragma once

#include <sb_core/core.h>
#include <sb_core/compiler.h>

namespace sb {

// returns number of characters copied without the terminating '\0'
usize strCpyT(char * __sb_restrict output, usize capacity, char const * __sb_restrict src);

// returns number of characters copied without the terminating '\0'
template <usize COUNT>
inline usize strCpyT(char (&output)[COUNT], char const * __sb_restrict src)
{
    return strCpyT(&output[0], COUNT, src);
}

char * strCatT(char * __sb_restrict dst, usize dst_len, usize dst_capacity, char const * __sb_restrict concat);

// returns true if 'str' begins with 'start_str'
b8 strStartsWith(char const * __sb_restrict str, char const * __sb_restrict start_str);

} // namespace sb
