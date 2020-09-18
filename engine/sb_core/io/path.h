#pragma once

#include <sb_core/core.h>
#include <sb_core/os.h>

#include <sb_std/span>

namespace sb {

inline constexpr ui32 VFS_PATH_MAX_LEN = 255U;
inline constexpr char VFS_PATH_SEPARATOR = '/';

inline constexpr ui32 PHYS_PATH_MAX_LEN = internal::OS_FILE_PATH_MAX_LEN;
inline constexpr char PHYS_PATH_SEPARATOR = internal::OS_FILE_PATH_SEPARATOR;

b8 isVfsPathValid(char const * vfs_path);

char * concatPhysPath(sbstd::span<char> base_path, char const * path_cat);

} // namespace sb
