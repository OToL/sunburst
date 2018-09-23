#pragma once

#include <core/platform.h>

#define SB_HASHSTR_POLICY_CRC 1
#define SB_HASHSTR_POLICY_FNV1A 2
#define SB_CTV_HASHSTR_POLICY SB_HASHSTR_POLICY_FNV1A

#if sbCTVIsEq(HASHSTR_POLICY, SB_HASHSTR_POLICY_CRC)

#    include <core/hash/crc.h>

namespace sb {

template <usize BIT_COUNT>
using HashStrPolicy = HashPolicyCRC<BIT_COUNT>;
}

#elif sbCTVIsEq(HASHSTR_POLICY, SB_HASHSTR_POLICY_FNV1A)

#    include <core/hash/fnv.h>

namespace sb {

template <usize SIZE>
using HashStrPolicy = HashPolicyFNV1a<SIZE>;
}

#endif
