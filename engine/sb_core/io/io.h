#pragma once

#include <sb_core/core.h>
#include <sb_core/os.h>

namespace sb {

enum class FileWriteMode : ui8
{
    TRUNC,
    APPEND
};

enum class FileFormat : ui8
{
    Bin,
    Text
};

using FileSize = si64;
using FilePos = si64;

inline constexpr ui32 VFS_LAYER_NAME_MAX_LEN = 10U;
inline constexpr ui32 VFS_LAYER_MAX_COUNT = 30U;
inline constexpr usize VFS_FILE_MAX_OPENED = 50U;
inline constexpr ui32 VFS_PATH_MAX_LEN = 255U;
inline constexpr char VFS_PATH_SEPARATOR = '/';

inline constexpr ui32 LOCAL_PATH_MAX_LEN = internal::OS_FILE_PATH_MAX_LEN;
inline constexpr char LOCAL_PATH_SEPARATOR = internal::OS_FILE_PATH_SEPARATOR;

} // namespace sb
