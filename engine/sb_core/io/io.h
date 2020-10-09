#pragma once

#include <sb_core/core.h>
#include <sb_core/os.h>
#include <sb_core/enum.h>

namespace sb {

enum class FileWriteMode : u8
{
    TRUNC,
    APPEND
};

enum class FileFormat : u8
{
    BIN,
    TEXT
};

enum class FileAccess : u8
{
    READ,
    WRITE
};

struct FileProps
{
    FileFormat fmt;
    EnumMask<FileAccess> access;
};

using FileSize = s64;
using FilePos = s64;

inline constexpr u32 VFS_LAYER_NAME_MAX_LEN = 10U;
inline constexpr u32 VFS_LAYER_MAX_COUNT = 30U;
inline constexpr usize VFS_FILE_MAX_OPENED = 50U;
inline constexpr u32 VFS_PATH_MAX_LEN = 255U;
inline constexpr char VFS_PATH_SEPARATOR = '/';

inline constexpr u32 LOCAL_PATH_MAX_LEN = internal::OS_FILE_PATH_MAX_LEN;
inline constexpr char LOCAL_PATH_SEPARATOR = internal::OS_FILE_PATH_SEPARATOR;

} // namespace sb
