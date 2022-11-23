#pragma once

#include <sb_core/core.h>
#include <sb_core/system.h>

#if defined(SB_PLATFORM_WINDOWS)
#    include <sb_core/_impl/io/io_win.h>
#else
#    error "Unhandled platform"
#endif

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

enum class FileSystemType : u8
{
    LOCAL
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

using internal::FS_PATH_SEPERATOR;
using internal::FS_PATH_INVALID_SEPERATOR;
using internal::FS_PATH_MAX_LEN;

inline constexpr usize VFS_FILE_MAX_OPENED = 50U;
inline constexpr u32 VFS_PATH_MAX_LEN = 255U;
inline constexpr char VFS_PATH_SEPARATOR = '/';
inline constexpr u32 VFS_LAYER_NAME_MAX_LEN = 10U;
inline constexpr u32 VFS_LAYER_MAX_COUNT = 30U;

} // namespace sb
