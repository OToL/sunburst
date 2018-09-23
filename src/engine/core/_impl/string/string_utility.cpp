#include <core/utility.h>

namespace sb {

usize strCpyT(char * sb_restrict output, usize capacity, char const * sb_restrict src)
{
    if (0 == capacity)
    {
        return 0;
    }

    usize copy_count = 0;
    char const * sb_restrict srcIter = src;

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

bool strStartWith(char const * sb_restrict str, char const * sb_restrict start_str)
{
    char const * sb_restrict src_iter = str;
    char const * sb_restrict start_str_iter = start_str;

    while ((*src_iter == *start_str_iter) && ('\0' != *src_iter) && ('\0' != *start_str_iter))
    {
        ++src_iter;
        ++start_str_iter;
    }

    return ('\0' == *start_str_iter);
}

} // namespace sb
