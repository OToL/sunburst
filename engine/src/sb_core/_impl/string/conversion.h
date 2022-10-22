#pragma once

#include <sb_slw/span>

namespace sb::internal {

usize stringToCharBuffer(char const * src, slw::span<char> dst);

template <typename T>
usize decimalToString(T src, slw::span<char> dest);

extern template usize decimalToString<i64>(i64 src, slw::span<char> dest);
extern template usize decimalToString<i32>(i32 src, slw::span<char> dest);
extern template usize decimalToString<i16>(i16 src, slw::span<char> dest);
extern template usize decimalToString<i8>(i8 src, slw::span<char> dest);
extern template usize decimalToString<u64>(u64 src, slw::span<char> dest);
extern template usize decimalToString<u32>(u32 src, slw::span<char> dest);
extern template usize decimalToString<u16>(u16 src, slw::span<char> dest);
extern template usize decimalToString<u8>(u8 src, slw::span<char> dest);
extern template usize decimalToString<usize>(usize src, slw::span<char> dest);
extern template usize decimalToString<unsigned long>(unsigned long src, slw::span<char> dest);

} // namespace sb::internal
