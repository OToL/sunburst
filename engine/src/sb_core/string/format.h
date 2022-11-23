#pragma once

#include <sb_core/core.h>

#include <sb_slw/forward_decls>

/// String formatting facility inspired by fmtlib:
/// - https://docs.python.org/2/library/string.html
/// - https://github.com/fmtlib/fmt

namespace sb {

/// Returns the number of characters copied without the null terminated character ('\0')
template <typename... TArgs>
usize formatString(slw::span_fw<char> const &dest_buffer, char const * const format, TArgs &&... args);

} // namespace sb

#include <sb_core/_impl/string/format.h>
