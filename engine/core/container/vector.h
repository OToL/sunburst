#pragma once

#include <core/memory/allocator/stl_allocator.h>

#include <wstd/vector>

namespace sb {

template <typename T, typename TAllocator = STLAllocator<T>>
using Vector = wstd::internal::vector<T, TAllocator>;

}
