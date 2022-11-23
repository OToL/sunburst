#pragma once

#include <sb_slw/_impl/array>

namespace sb {

template <typename T, size_t Size>
using Array = slw::internal::array<T, Size>;

} // namespace sb

