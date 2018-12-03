#pragma once

#include <core/platform.h>

#include <libc++/span>

namespace sb {

constexpr usize LOGICAL_PATH_MAX_LEN = 255;
constexpr char const * LOGICAL_PATH_SEPARATOR = "/";

b8 isLogicalPathValid(char const * path);

char * concatPhysicalPaths(wstd::span<char> base_path, char const * path_cat);

} // namespace sb
