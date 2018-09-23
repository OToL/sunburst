#pragma once

#include <core/platform.h>

namespace sb {

constexpr usize LOGICAL_PATH_MAX_LEN = 255;
constexpr char const * LOGICAL_PATH_SEPARATOR = "/";

ui8 isLogicalPathValid(char const * path);

} // namespace sb
