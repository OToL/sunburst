#include <sb_core/string/string_utility.h>
#include <sb_core/error.h>

char * sb::strCatT(char * _sb_noalias_ dst, usize dst_len, usize dst_capacity, char const * _sb_noalias_ concat)
{
    sbAssert(dst_len <= dst_capacity);
    sbAssert(dst[dst_len] == 0);

    strCpyT(dst + dst_len, dst_capacity - dst_len, concat);

    return dst;
}

sb::usize sb::strCpyT(char * _sb_noalias_ output, usize capacity, char const * _sb_noalias_ src)
{
    if (0 == capacity)
    {
        return 0;
    }

    usize copy_count = 0;
    char const * _sb_noalias_ srcIter = src;

    do
    {
        output[copy_count] = *srcIter;
        ++copy_count;
    } while ((*(srcIter++) != '\0') && (copy_count != capacity));

    if (0 != copy_count)
    {
        output[copy_count - 1] = '\0';
        --copy_count;
    }

    return copy_count;
}

sb::b8 sb::strStartsWith(char const * _sb_noalias_ str, char const * _sb_noalias_ start_str)
{
    char const * _sb_noalias_ src_iter = str;
    char const * _sb_noalias_ start_str_iter = start_str;

    while ((*src_iter == *start_str_iter) && ('\0' != *src_iter) && ('\0' != *start_str_iter))
    {
        ++src_iter;
        ++start_str_iter;
    }

    return ('\0' == *start_str_iter);
}
