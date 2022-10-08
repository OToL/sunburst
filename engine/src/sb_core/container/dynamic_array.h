#pragma once
#include <sb_core/memory/allocator/stl_allocator.h>

#include <sb_std/_impl/vector>

namespace sb {

template <typename T, typename TAllocator = STLAllocator<T>>
using DynamicArray = sbstd::internal::vector<T, TAllocator>;

template <typename T, typename TAllocator = STLAllocator<T>>
using DArray = DynamicArray<T, TAllocator>;

} // namespace sb
