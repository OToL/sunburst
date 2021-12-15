#pragma once

#include <sb_std/cstddef>
#include <sb_std/cstdint>

namespace sb {

using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
using uptr = uintptr_t;
using iptrdiff = ptrdiff_t;
using usize = size_t;
using f32 = float;
using f64 = double;
using b8 = bool;
using b32 = u32;
using Alignment = usize;

} // namespace sb

// Compile Time Feature ak CTF
#define SB_CTF_DISABLED 1
#define SB_CTF_ENABLED 2

#define sb_ctf_enabled(feature) (1 == 2 / SB_CTF_##feature)
#define sb_ctf_equal(value_name, value) (SB_CTF_##value_name == SB_CTF_##value_name##_##value)

#define sb_base(...) using Base = __VA_ARGS__

#define sb_copy_protect(entity_name) \
    entity_name(entity_name const &) = delete; \
    entity_name(entity_name &&) = delete; \
    entity_name & operator=(entity_name const &) = delete; \
    entity_name & operator=(entity_name &&) = delete
