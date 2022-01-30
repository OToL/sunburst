#pragma once

#include <sb_core/io/io.h>
#include <sb_core/core.h>

namespace sb {

struct File
{
    void * value;
};

struct FileSystem
{
    File (*open_file_read)(char const * path, FileFormat fmt);

    File (*open_file_write)(char const * path, FileWriteMode mode, FileFormat fmt);

    FileSize (*read_file)(File hdl, u8 * buffer, FileSize count);

    FileSize (*write_file)(File hdl, u8 const * buffer, FileSize count);

    FileSize (*get_file_length)(File hdl);

    void (*close_file)(File hdl);

    b8 (*file_exists)(char const * path);

    b8 (*is_file_valid)(File hdl);
};

} // namespace sb