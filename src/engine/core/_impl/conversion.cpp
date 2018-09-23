#include <core/conversion.h>
#include <core/utility.h>

namespace sb { namespace detail {

usize stringToCharBuffer(char const * src, Span<char> & dst)
{
    return sb::strCpyT(dst.data(), dst.size(), src);
}

// String cast functions are taken from folly:
// https://github.com/facebook/folly/blob/master/folly/Conv.h

// Return the exact number of digits in 'v'
inline ui32 digits10(ui64 v)
{
    // For this arch we can get a little help from specialized CPU instructions
    // which can count leading zeroes; 64 minus that is appx. log (base 2).
    // Use that to approximate base-10 digits (log_10) and then adjust if needed.

    // 10^i, defined for i 0 through 19.
    // This is 20 * 8 == 160 bytes, which fits neatly into 5 cache lines
    // (assuming a cache line size of 64).
    static const ui64 POWERS_OF_10[20] = {
        1,
        10,
        100,
        1000,
        10000,
        100000,
        1000000,
        10000000,
        100000000,
        1000000000,
        10000000000,
        100000000000,
        1000000000000,
        10000000000000,
        100000000000000,
        1000000000000000,
        10000000000000000,
        100000000000000000,
        1000000000000000000,
        10000000000000000000UL,
    };

    // "count leading zeroes" operation not valid; for 0; special case this.
    if (0 == v)
    {
        return 1;
    }

    // bits is in the ballpark of log_2(v).
    const ui8 leading_zeroes = (ui8)__builtin_clzll(v);
    const ui32 bits = (63 - leading_zeroes);

    // approximate log_10(v) == log_10(2) * bits.
    // Integer magic below: 77/256 is appx. 0.3010 (log_10(2)).
    // The +1 is to make this the ceiling of the log_10 estimate.
    const ui32 min_length = (1 + ((bits * 77) >> 8));

    // return that log_10 lower bound, plus adjust if input >= 10^(that bound)
    // in case there's a small error and we misjudged length.
    return (min_length + (uint32_t)(v >= POWERS_OF_10[min_length]));
}

inline ui32 convertToBuffer(ui64 v, char * const buffer, usize capacity)
{
    if (0 == capacity)
    {
        return 0;
    }

    auto const result = digits10(v);

    // WARNING: using size_t or pointer arithmetic for pos slows down
    // the loop below 20x. This is because several 32-bit ops can be
    // done in parallel, but only fewer 64-bit ones.
    ui32 pos = result - 1;

    if (result >= capacity)
    {
        buffer[0] = '\0';
        return 0;
    }

    buffer[result] = '\0';

    while (v >= 10)
    {
        // Keep these together so a peephole optimization "sees" them and
        // computes them in one shot.
        auto const q = v / 10;
        auto const r = static_cast<char>(v % 10);

        buffer[pos--] = '0' + r;
        v = q;
    }

    // Last digit is trivial to handle
    buffer[pos] = static_cast<char>(v) + '0';

    return result;
}

template <typename T>
typename wstd::enable_if_t<wstd::is_signed<T>::value, usize> decimalToStringImpl(T src, sb::Span<char> & dest)
{
    if (!dest.empty())
    {
        if (src < 0)
        {
            if (2 <= dest.size())
            {
                dest[0] = '-';
                ui32 const bytes_cnt = convertToBuffer(ui64(-ui64(src)), dest.data() + 1, dest.size() - 1);

                if (0 != bytes_cnt)
                {
                    // +1 because of '-' character
                    return (bytes_cnt + 1);
                }
            }
        }
        else
        {
            ui32 const bytes_cnt = convertToBuffer(ui64(src), dest.data(), dest.size());

            if (0 != bytes_cnt)
            {
                return bytes_cnt;
            }
        }

        dest[0] = '\0';
    }

    return 0;
}

template <typename T>
typename wstd::enable_if_t<!wstd::is_signed<T>::value, usize> decimalToStringImpl(T src, sb::Span<char> & dest)
{
    auto const bytes_cnt = convertToBuffer(ui64(src), dest.data(), dest.size());

    if (0 != bytes_cnt)
    {
        return bytes_cnt;
    }
    else
    {
        dest[0] = '\0';
        return 0;
    }
}

template <typename T>
usize decimalToString(T src, sb::Span<char> & dest)
{
    return decimalToStringImpl(src, dest);
}

template usize decimalToString<si64>(si64 src, sb::Span<char> & dest);
template usize decimalToString<si32>(si32 src, sb::Span<char> & dest);
template usize decimalToString<si16>(si16 src, sb::Span<char> & dest);
template usize decimalToString<si8>(si8 src, sb::Span<char> & dest);

template usize decimalToString<ui64>(ui64 src, sb::Span<char> & dest);
template usize decimalToString<ui32>(ui32 src, sb::Span<char> & dest);
template usize decimalToString<ui16>(ui16 src, sb::Span<char> & dest);
template usize decimalToString<ui8>(ui8 src, sb::Span<char> & dest);
// template usize decimalToString<uiptr>(uiptr const & src,
// sb::Span<char> & dest);
// template usize decimalToString<usize>(usize const & src, sb::Span<char> & dest);

static_assert(sizeof(usize) <= sizeof(ui64), "usize is too big for decimalToString which is casting "
                                              "integral values to sbU64 before conversion");
}} // namespace sb::detail
