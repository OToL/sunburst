#pragma once

namespace sb::internal {

usize stringToCharBuffer(char const * src, sbstd::span<char> dst);

template <typename T>
usize decimalToString(T src, sbstd::span<char> dest);

extern template usize decimalToString<s64>(s64 src, sbstd::span<char> dest);
extern template usize decimalToString<s32>(s32 src, sbstd::span<char> dest);
extern template usize decimalToString<s16>(s16 src, sbstd::span<char> dest);
extern template usize decimalToString<s8>(s8 src, sbstd::span<char> dest);
extern template usize decimalToString<u64>(u64 src, sbstd::span<char> dest);
extern template usize decimalToString<u32>(u32 src, sbstd::span<char> dest);
extern template usize decimalToString<u16>(u16 src, sbstd::span<char> dest);
extern template usize decimalToString<u8>(u8 src, sbstd::span<char> dest);
extern template usize decimalToString<usize>(usize src, sbstd::span<char> dest);
extern template usize decimalToString<unsigned long>(unsigned long src, sbstd::span<char> dest);

} // namespace sb::internal
