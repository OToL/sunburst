#pragma once

#include <sb_core/core.h>

#include <sb_slw/forward_decls>

namespace sb {

void getWorkingDirectory(slw::span_fw<char> const & buffer);
void outputDebugString(char const * msg);

} // namespace sb
