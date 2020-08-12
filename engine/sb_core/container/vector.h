#pragma once

#include <sb_core/memory/allocator/stl_allocator.h>

#include <sb_std/vector>

namespace sb {

template <typename T, typename TAllocator = STLAllocator<T>>
using Vector = sbstd::internal::vector<T, TAllocator>;

}
