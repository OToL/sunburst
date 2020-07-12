#pragma once

#include <core/base.h>

namespace sb {

struct FNV1a32
{
    using Value = ui32;

    static constexpr Value OFFSET = 0x811C9DC5U;
    static constexpr Value PRIME = 0x01000193U;
    static constexpr Value NULL_VALUE = OFFSET;

    static constexpr Value compute(ui8 const * const buffer, usize const len);
    static constexpr Value compute(char const * const str);
};

struct FNV1a64
{
    using Value = ui64;

    static constexpr Value OFFSET = 0xCBF29CE484222325ULL;
    static constexpr Value PRIME = 0x100000001b3ULL;
    static constexpr Value NULL_VALUE = OFFSET;

    static constexpr Value compute(ui8 const * const buffer, usize const len);
    static constexpr Value compute(char const * const str);
};

template <usize BIT_COUNT>
struct HashPolicyFNV1a
{
public:
    using ValueType = void;

    static constexpr ValueType compute(ui8 const * const buffer, usize const len);
    static constexpr ValueType compute(ui8 const * const str);
};

} // namespace sb

#include <core/_impl/hash/fnv1a.h>
