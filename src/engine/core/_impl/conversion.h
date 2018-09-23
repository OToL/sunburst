#pragma once

#include <libc++/cstring>

namespace sb { namespace detail {

usize stringToCharBuffer(char const * src, Span<char> & dst);

/// Integral to string conversion

template <typename T>
usize decimalToString(T src, sb::Span<char> & dest);

extern template usize decimalToString<si64>(si64 src, sb::Span<char> & dest);
extern template usize decimalToString<si32>(si32 src, sb::Span<char> & dest);
extern template usize decimalToString<si16>(si16 src, sb::Span<char> & dest);
extern template usize decimalToString<si8>(si8 src, sb::Span<char> & dest);
extern template usize decimalToString<ui64>(ui64 src, sb::Span<char> & dest);
extern template usize decimalToString<ui32>(ui32 src, sb::Span<char> & dest);
extern template usize decimalToString<ui16>(ui16 src, sb::Span<char> & dest);
extern template usize decimalToString<ui8>(ui8 src, sb::Span<char> & dest);
extern template usize decimalToString<usize>(usize src, sb::Span<char> & dest);
// extern template usize decimalToString<uiptr>(uiptr src, sb::Span<char> & dest);

}} // namespace sb::detail
