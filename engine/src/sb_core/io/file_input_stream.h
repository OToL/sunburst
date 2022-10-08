#pragma once

#include "io.h"
#include "file_hdl.h"
#include <sb_core/memory/memory_arena.h>

#include <sb_std/span>

namespace sb {

class FileInputStream
{
public:
    FileInputStream() = default;

    explicit FileInputStream(FileHdl hdl, usize buffer_size = 65536);

    FileInputStream(FileInputStream && src) = delete;
    FileInputStream & operator=(FileInputStream && src) = delete;

    FileInputStream(FileInputStream const &) = delete;
    FileInputStream & operator=(FileInputStream const &) = delete;

    ~FileInputStream();

    b8 isValid() const
    {
        return _hdl.isValid();
    }

    FileSize read(sbstd::span<u8> buffer, FileSize cnt = -1);

    FileSize getLength();

    void reset(FileHdl hdl = {});

    FileHdl swap(FileHdl hdl);

private:
    FileHdl _hdl;
    MemoryArena _buffer;
    FileSize _buffer_fetch_size = 0;
    FilePos _buffer_curr_offset = 0;
    //FilePos _buffer_base_file_offset = 0;
    FileSize _cached_file_length = -1;
};

} // namespace sb
