#include <sb_core/core.h>
#include <sb_core/conversion.h>
#include <sb_core/error/error.h>
#include <sb_core/string/string_utility.h>

#include <sb_std/charconv>
#include <sb_std/span>

sb::usize sb::internal::stringToCharBuffer(char const * src, sbstd::span<char> dst)
{
    return sb::strCpyT(dst.data(), truncValue<usize>(dst.size()), src);
}

template <typename T>
sb::usize sb::internal::decimalToString(T src, sbstd::span<char> dest)
{
    sbstd::to_chars_result const res = sbstd::to_chars(dest.data(), dest.data() + dest.size(), src);

    if (res.ec == sbstd::errc{})
    {
        auto const len = truncValue<usize>(res.ptr - dest.data());

        if (0 != len)
        {
            if (sb_expected(len < dest.size()))
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

        return truncValue<usize>(len);
    }

    return 0;
}

template sb::usize sb::internal::decimalToString<sb::i64>(i64 src, sbstd::span<char> dest);
template sb::usize sb::internal::decimalToString<sb::i32>(i32 src, sbstd::span<char> dest);
template sb::usize sb::internal::decimalToString<sb::i16>(i16 src, sbstd::span<char> dest);
template sb::usize sb::internal::decimalToString<sb::i8>(i8 src, sbstd::span<char> dest);

template sb::usize sb::internal::decimalToString<sb::u64>(u64 src, sbstd::span<char> dest);
template sb::usize sb::internal::decimalToString<sb::u32>(u32 src, sbstd::span<char> dest);
template sb::usize sb::internal::decimalToString<sb::u16>(u16 src, sbstd::span<char> dest);
template sb::usize sb::internal::decimalToString<sb::u8>(u8 src, sbstd::span<char> dest);
template sb::usize sb::internal::decimalToString<unsigned long>(unsigned long src, sbstd::span<char> dest);
