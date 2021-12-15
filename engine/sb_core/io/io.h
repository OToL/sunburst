#pragma once

#include <sb_core/core.h>
#include <sb_core/os.h>

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

struct FileAccessFlags 
{ 
    using ValueType = u8; 
    enum : ValueType 
    { 
        READ = 1 << 0,
        WRITE = 1 << 1
    }; 

    ValueType value; 
};

struct FileProps
{
    FileFormat fmt;
    FileAccessFlags access;
};

using FileSize = i64;
using FilePos = i64;

inline constexpr u32 VFS_LAYER_NAME_MAX_LEN = 10U;
inline constexpr u32 VFS_LAYER_MAX_COUNT = 30U;
inline constexpr usize VFS_FILE_MAX_OPENED = 50U;
inline constexpr u32 VFS_PATH_MAX_LEN = 255U;
inline constexpr char VFS_PATH_SEPARATOR = '/';

} // namespace sb
