#pragma once
#include <sb_core/memory/allocators/stl_allocator.h>

#include <sb_slw/_impl/vector>

namespace sb {

template <typename T, typename TAllocator = STLAllocator<T>>
using DynamicArray = slw::internal::vector<T, TAllocator>;

template <typename T, typename TAllocator = STLAllocator<T>>
using DArray = DynamicArray<T, TAllocator>;

} // namespace sb
