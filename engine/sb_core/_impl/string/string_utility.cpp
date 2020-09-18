#include <sb_core/string/utility.h>

sb::usize sb::strCpyT(char * __restrict output, usize capacity, char const * __restrict src)
{
    if (0 == capacity)
    {
        return 0;
    }

    usize copy_count = 0;
    char const * __restrict srcIter = src;

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

sb::b8 sb::strStartWith(char const * __restrict str, char const * __restrict start_str)
{
    char const * __restrict src_iter = str;
    char const * __restrict start_str_iter = start_str;

    while ((*src_iter == *start_str_iter) && ('\0' != *src_iter) && ('\0' != *start_str_iter))
    {
        ++src_iter;
        ++start_str_iter;
    }

    return ('\0' == *start_str_iter);
}
