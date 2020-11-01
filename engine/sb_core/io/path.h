#pragma once

#include "io.h"
#include <sb_core/core.h>

#include <sb_std/span>

namespace sb {

b8 isVFSPathValid(char const * vfs_path);

char * concatLocalPath(sbstd::span<char> base_path, char const * path_cat);
char * concatLocalPath(sbstd::span<char> base_path, usize base_path_len, char const * path_cat);
char * normalizeLocalPath(char * path);

} // namespace sb
