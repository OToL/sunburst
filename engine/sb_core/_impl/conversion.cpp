#include <sb_core/core.h>
#include <sb_core/conversion.h>
#include <sb_core/error.h>
#include <sb_core/string/utility.h>

#include <sb_std/charconv>
#include <sb_std/span>
#include <sb_std/algorithm>
#include <sb_std/utility>

namespace sb::internal {

usize stringToCharBuffer(char const * src, sbstd::span<char> dst)
{
    return sb::strCpyT(dst.data(), numericConv<usize>(dst.size()), src);
}

template <typename T>
usize decimalToString(T src, sbstd::span<char> dest)
{
    sbstd::to_chars_result const res = sbstd::to_chars(dest.data(), dest.data() + dest.size(), src);

    if (res.ec == sbstd::errc{})
    {
        auto const len = numericConv<usize>(sbstd::distance(dest.data(), res.ptr));

        if (0 != len)
        {
            if (sbExpect(len < dest.size()))
            {
                dest[len] = 0;
            }
            else
            {
                dest[len - 1] = 0;
            }
        }
        else
        {
            dest[0] = 0;
        }

        return numericConv<usize>(len);
    }

    return 0;
}

template usize decimalToString<si64>(si64 src, sbstd::span<char> dest);
template usize decimalToString<si32>(si32 src, sbstd::span<char> dest);
template usize decimalToString<si16>(si16 src, sbstd::span<char> dest);
template usize decimalToString<si8>(si8 src, sbstd::span<char> dest);

template usize decimalToString<ui64>(ui64 src, sbstd::span<char> dest);
template usize decimalToString<ui32>(ui32 src, sbstd::span<char> dest);
template usize decimalToString<ui16>(ui16 src, sbstd::span<char> dest);
template usize decimalToString<ui8>(ui8 src, sbstd::span<char> dest);

} // namespace sb::internal
