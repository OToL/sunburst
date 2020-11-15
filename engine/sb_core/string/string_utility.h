#pragma once

#include <sb_core/core.h>
#include <sb_core/compiler.h>

namespace sb {

// returns number of characters copied without the terminating '\0'
usize strCpyT(char * _sb_noalias_ output, usize capacity, char const * _sb_noalias_ src);

// returns number of characters copied without the terminating '\0'
template <usize COUNT>
inline usize strCpyT(char (&output)[COUNT], char const * _sb_noalias_ src)
{
    return strCpyT(&output[0], COUNT, src);
}

char * strCatT(char * _sb_noalias_ dst, usize dst_len, usize dst_capacity, char const * _sb_noalias_ concat);

// returns true if 'str' begins with 'start_str'
b8 strStartsWith(char const * _sb_noalias_ str, char const * _sb_noalias_ start_str);

} // namespace sb
