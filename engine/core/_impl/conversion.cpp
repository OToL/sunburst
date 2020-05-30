#include <core/types.h>
#include <core/conversion.h>
#include <core/error.h>
#include <core/string/utility.h>

#include <libc++/charconv>
#include <libc++/span>
#include <libc++/algorithm>
#include <libc++/utility>

namespace sb::detail {

usize stringToCharBuffer(char const * src, wstd::span<char> & dst)
{
    return sb::strCpyT(dst.data(), numericCast<usize>(dst.size()), src);
}

template <typename T>
usize decimalToString(T src, wstd::span<char> & dest)
{
    wstd::to_chars_result const res = wstd::to_chars(dest.data(), dest.data() + dest.size(), src);

    if (res.ec == wstd::errc{})
    {
        auto const len = wstd::distance(dest.data(), res.ptr);

        if (0 != len)
        {
            if (sbExpectTrue(len < dest.size()))
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

        return numericCast<usize>(len);
    }

    return 0;
}

template usize decimalToString<si64>(si64 src, wstd::span<char> & dest);
template usize decimalToString<si32>(si32 src, wstd::span<char> & dest);
template usize decimalToString<si16>(si16 src, wstd::span<char> & dest);
template usize decimalToString<si8>(si8 src, wstd::span<char> & dest);

template usize decimalToString<ui64>(ui64 src, wstd::span<char> & dest);
template usize decimalToString<ui32>(ui32 src, wstd::span<char> & dest);
template usize decimalToString<ui16>(ui16 src, wstd::span<char> & dest);
template usize decimalToString<ui8>(ui8 src, wstd::span<char> & dest);

} // namespace sb::detail
