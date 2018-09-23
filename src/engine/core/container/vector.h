#pragma once

#include <core/memory/allocator/stl_allocator.h>

#include <libc++/vector>

namespace sb {

    template <typename T, typename TAllocator = STLAllocator<T>>
    using Vector = wstd::priv::vector<T, TAllocator>;

}

