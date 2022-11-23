#pragma once

#include "io.h"
#include <sb_core/core.h>

#include <sb_slw/forward_decls>

namespace sb {

// virtual file system path utilities
b8 isVfsPathValid(char const * vfs_path);

// file system path operations utilities
b8 isFsPathValid(char const * local_path);
char * concatFsPath(slw::span_fw<char> const &base_path, char const * path_cat);
char * concatFsPath(slw::span_fw<char> const &base_path, usize base_path_len, char const * path_cat);
char * normalizeFsPath(char * path);

} // namespace sb
