#pragma once

#include <core/types.h>
#include <core/compiler.h>

#include <wstd/span>

/// String formatting facility inspired by fmtlib:
/// - https://docs.python.org/2/library/string.html
/// - https://github.com/fmtlib/fmt

namespace sb {

/// Returns the number of characters copied without the null terminated character ('\0')
template <typename... TArgs>
inline usize stringFormat(wstd::span<char> dest_buffer, char const * const format,
                          TArgs &&... args);

} // namespace sb

#include <core/_impl/string/string_format.h>
