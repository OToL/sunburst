#pragma once

#include <core/platform.h>

namespace sb {

using CRC32Value = ui32;
using CRC64Value = ui64;

constexpr CRC32Value CRC32_NULL = 0U;
constexpr CRC64Value CRC64_NULL = 0ULL;

CRC32Value computeCRC32(ui8 const * const buffer, usize const len);

CRC32Value computeCRC32(char const * const str);

CRC64Value computeCRC64(ui8 const * const buffer, usize const len);

CRC64Value computeCRC64(char const * const str);

template <usize BIT_COUNT>
struct HashPolicyCRC
{
public:
    using ValueType = void;

    static ValueType compute(ui8 const * const buffer, usize const len);
    static ValueType compute(char const * const str);
};

} // namespace sb

#include <core/_impl/hash/crc.h>
