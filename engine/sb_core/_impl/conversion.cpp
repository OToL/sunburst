#include <sb_core/core.h>
#include <sb_core/conversion.h>
#include <sb_core/error.h>
#include <sb_core/string/utility.h>

#include <sb_std/charconv>
#include <sb_std/span>
#include <sb_std/algorithm>
#include <sb_std/utility>

sb::usize sb::internal::stringToCharBuffer(char const * src, sbstd::span<char> dst)
{
    return sb::strCpyT(dst.data(), numericConv<usize>(dst.size()), src);
}

template <typename T>
sb::usize sb::internal::decimalToString(T src, sbstd::span<char> dest)
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

template sb::usize sb::internal::decimalToString<sb::si64>(si64 src, sbstd::span<char> dest);
template sb::usize sb::internal::decimalToString<sb::si32>(si32 src, sbstd::span<char> dest);
template sb::usize sb::internal::decimalToString<sb::si16>(si16 src, sbstd::span<char> dest);
template sb::usize sb::internal::decimalToString<sb::si8>(si8 src, sbstd::span<char> dest);

template sb::usize sb::internal::decimalToString<sb::ui64>(ui64 src, sbstd::span<char> dest);
template sb::usize sb::internal::decimalToString<sb::ui32>(ui32 src, sbstd::span<char> dest);
template sb::usize sb::internal::decimalToString<sb::ui16>(ui16 src, sbstd::span<char> dest);
template sb::usize sb::internal::decimalToString<sb::ui8>(ui8 src, sbstd::span<char> dest);
