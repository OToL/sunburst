#pragma once

namespace sb::internal {

usize stringToCharBuffer(char const * src, sbstd::span<char> dst);

template <typename T>
usize decimalToString(T src, sbstd::span<char> dest);

extern template usize decimalToString<i64>(i64 src, sbstd::span<char> dest);
extern template usize decimalToString<i32>(i32 src, sbstd::span<char> dest);
extern template usize decimalToString<i16>(i16 src, sbstd::span<char> dest);
extern template usize decimalToString<i8>(i8 src, sbstd::span<char> dest);
extern template usize decimalToString<u64>(u64 src, sbstd::span<char> dest);
extern template usize decimalToString<u32>(u32 src, sbstd::span<char> dest);
extern template usize decimalToString<u16>(u16 src, sbstd::span<char> dest);
extern template usize decimalToString<u8>(u8 src, sbstd::span<char> dest);
extern template usize decimalToString<usize>(usize src, sbstd::span<char> dest);
extern template usize decimalToString<unsigned long>(unsigned long src, sbstd::span<char> dest);

} // namespace sb::internal
