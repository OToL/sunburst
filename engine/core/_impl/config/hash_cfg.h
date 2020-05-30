#pragma once

#include <core/types.h>

#define SB_HASHSTR_POLICY_CRC 1
#define SB_HASHSTR_POLICY_FNV1A 2
#define SB_CTV_HASHSTR_POLICY SB_HASHSTR_POLICY_FNV1A

#if sbCTVIsEq(HASHSTR_POLICY, SB_HASHSTR_POLICY_CRC)

#    include <core/hash/crc.h>

namespace sb::detail {

using HashStrPolicy = CRC64;

}

#elif sbCTVIsEq(HASHSTR_POLICY, SB_HASHSTR_POLICY_FNV1A)

#    include <core/hash/fnv1a.h>

namespace sb::detail {

using HashStrPolicy = FNV1a64;

}

#endif
