#include <sb_core/core.h>
#include <sb_core/cast.h>
#include <sb_core/error.h>
#include <sb_core/string/conversion.h>
#include <sb_core/string/utility.h>

#include <sb_slw/charconv>
#include <sb_slw/span>

sb::usize sb::internal::stringToCharBuffer(char const * src, slw::span<char> const &dst)
{
    return sb::copyStringT(dst.data(), integral_cast<usize>(dst.size()), src);
}

template <typename T>
sb::usize sb::internal::decimalToString(T src, slw::span_fw<char> const &dest)
{
    slw::to_chars_result const res = slw::to_chars(dest.data(), dest.data() + dest.size(), src);

    if (res.ec == slw::errc{})
    {
        auto const len = integral_cast<usize>(res.ptr - dest.data());

        if (0 != len)
        {
            if (sb_expect(len < dest.size()))
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

        return integral_cast<usize>(len);
    }

    return 0;
}

template sb::usize sb::internal::decimalToString<sb::i64>(i64 src, slw::span_fw<char> const &dest);
template sb::usize sb::internal::decimalToString<sb::i32>(i32 src, slw::span_fw<char> const &dest);
template sb::usize sb::internal::decimalToString<sb::i16>(i16 src, slw::span_fw<char> const &dest);
template sb::usize sb::internal::decimalToString<sb::i8>(i8 src, slw::span_fw<char> const &dest);

template sb::usize sb::internal::decimalToString<sb::u64>(u64 src, slw::span_fw<char> const &dest);
template sb::usize sb::internal::decimalToString<sb::u32>(u32 src, slw::span_fw<char> const &dest);
template sb::usize sb::internal::decimalToString<sb::u16>(u16 src, slw::span_fw<char> const &dest);
template sb::usize sb::internal::decimalToString<sb::u8>(u8 src, slw::span_fw<char> const &dest);
template sb::usize sb::internal::decimalToString<unsigned long>(unsigned long src, slw::span_fw<char> const &dest);
