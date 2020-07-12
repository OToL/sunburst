#pragma once

#include <core/types.h>

#include <wstd/cstring>
#include <wstd/span>

namespace sb { namespace detail {

    usize stringToCharBuffer(char const * src, wstd::span<char> & dst);

    template <typename T>
    usize decimalToString(T src, wstd::span<char> & dest);

    extern template usize decimalToString<si64>(si64 src, wstd::span<char> & dest);
    extern template usize decimalToString<si32>(si32 src, wstd::span<char> & dest);
    extern template usize decimalToString<si16>(si16 src, wstd::span<char> & dest);
    extern template usize decimalToString<si8>(si8 src, wstd::span<char> & dest);
    extern template usize decimalToString<ui64>(ui64 src, wstd::span<char> & dest);
    extern template usize decimalToString<ui32>(ui32 src, wstd::span<char> & dest);
    extern template usize decimalToString<ui16>(ui16 src, wstd::span<char> & dest);
    extern template usize decimalToString<ui8>(ui8 src, wstd::span<char> & dest);
    extern template usize decimalToString<usize>(usize src, wstd::span<char> & dest);

}} // namespace sb::detail
