#pragma once

namespace sb::internal {

usize stringToCharBuffer(char const * src, sbstd::span<char> dst);

template <typename T>
usize decimalToString(T src, sbstd::span<char> dest);

extern template usize decimalToString<si64>(si64 src, sbstd::span<char> dest);
extern template usize decimalToString<si32>(si32 src, sbstd::span<char> dest);
extern template usize decimalToString<si16>(si16 src, sbstd::span<char> dest);
extern template usize decimalToString<si8>(si8 src, sbstd::span<char> dest);
extern template usize decimalToString<ui64>(ui64 src, sbstd::span<char> dest);
extern template usize decimalToString<ui32>(ui32 src, sbstd::span<char> dest);
extern template usize decimalToString<ui16>(ui16 src, sbstd::span<char> dest);
extern template usize decimalToString<ui8>(ui8 src, sbstd::span<char> dest);
extern template usize decimalToString<usize>(usize src, sbstd::span<char> dest);

}
