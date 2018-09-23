#pragma once

#include <core/platform.h>

namespace sb {

using FNV32Value = ui32;
using FNV64Value = ui64;

constexpr FNV32Value FNV32_OFFSET = 0x811C9DC5U;
constexpr FNV32Value FNV32_PRIME = 0x01000193U;
constexpr FNV32Value FNV32_NULL = FNV32_OFFSET;

constexpr FNV64Value FNV64_OFFSET = 0xCBF29CE484222325ULL;
constexpr FNV64Value FNV64_PRIME = 0x100000001b3ULL;
constexpr FNV64Value FNV64_NULL = FNV64_OFFSET;

constexpr FNV32Value computeFNV1a32(ui8 const * const buffer, usize const len);

constexpr FNV32Value computeFNV1a32(char const * const str);

constexpr FNV64Value computeFNV1a64(ui8 const * const buffer, usize const len);

constexpr FNV64Value computeFNV1a64(char const * const str);

template <usize BIT_COUNT>
struct HashPolicyFNV1a
{
public:
    using ValueType = void;

    static constexpr ValueType compute(ui8 const * const buffer, usize const len);
    static constexpr ValueType compute(ui8 const * const str);
};

} // namespace sb

#include <core/_impl/hash/fnv.h>
