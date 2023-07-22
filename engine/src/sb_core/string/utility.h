#pragma once

#include <sb_core/core.h>
#include <sb_core/compiler.h>

namespace sb {

// returns number of characters copied without the terminating '\0'
usize copyStringT(char * sb_noalias output, usize capacity, char const * sb_noalias src);

// returns number of characters copied without the terminating '\0'
template <usize COUNT>
inline usize copyStringT(char (&output)[COUNT], char const * sb_noalias src)
{
    return copyStringT(&output[0], COUNT, src);
}

char * concatStringT(char * sb_noalias dst, usize dst_len, usize dst_capacity, char const * sb_noalias concat);

// returns true if 'str' begins with 'start_str'
b8 strStartsWith(char const * sb_noalias str, char const * sb_noalias start_str);

} // namespace sb
