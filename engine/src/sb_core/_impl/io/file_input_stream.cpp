#include <sb_core/io/file_input_stream.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/error.h>
#include <sb_core/utility.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>

sb::FileInputStream::FileInputStream(FileHdl hdl, usize buffer_size)
    : _hdl(hdl)
{
    if (0 != buffer_size)
    {
        _buffer = sb_malloc(GHEAP, buffer_size);
    }
}

// sb::FileInputStream & sb::FileInputStream::operator=(FileInputStream && src)
//{
//    if (this != &src)
//    {
//        reset(src._hdl);
//        _buffer_curr_offset = src._buffer_curr_offset;
//
//        if (memory_arena::isValid(_buffer))
//        {
//            sb_free(GHEAP, _buffer);
//        }
//
//        _buffer = src._buffer;
//        _file_offset = src._file_offset;
//
//        src._hdl = {};
//        src._buffer = {};
//    }
//
//    return *this;
//}
//
// sb::FileInputStream::FileInputStream(FileInputStream && src)
//    : _hdl(src._hdl)
//    , _buffer(src._buffer)
//    , _buffer_curr_offset(src._buffer_curr_offset)
//    , _file_offset(src._file_offset)
//{
//    src._hdl = {};
//    src._buffer = {};
//}

sb::FileInputStream::~FileInputStream()
{
    reset();

    if (memory_arena::isValid(_buffer))
    {
        sb_free(GHEAP, _buffer);
    }
}

sb::FileSize sb::FileInputStream::read(sbstd::span<u8> buffer, FileSize cnt)
{
    if (_hdl.isValid() && !buffer.empty())
    {
        if (memory_arena::isValid(_buffer))
        {
            FileSize const requested_size = (-1 == cnt) ? integral_cast<FileSize>(buffer.size()) : cnt;
            FileSize read_size = 0;

            if (_buffer_curr_offset != _buffer_fetch_size)
            {
                auto const buffered_read_cnt = minValue(_buffer_fetch_size - _buffer_curr_offset, requested_size);
                read_size = buffered_read_cnt;

                memcpy(buffer.data(), reinterpret_cast<u8 const *>(_buffer.data) + _buffer_curr_offset,
                       integral_cast<usize>(buffered_read_cnt));
                _buffer_curr_offset += buffered_read_cnt;

                if (read_size == requested_size)
                {
                    return read_size;
                }
            }

            auto immediate_read_cnt = (requested_size - read_size) / _buffer.size;
            FileSize last_immediate_read_cnt = 0;
            bool isEOF = false;

            while ((read_size != requested_size) && (0 != immediate_read_cnt) && !isEOF)
            {
                FileSize const vfs_read_cnt =
                    vfs::readFile(_hdl, {reinterpret_cast<u8 *>(_buffer.data) + read_size, _buffer.size});
                read_size += vfs_read_cnt;
                isEOF = (vfs_read_cnt != integral_cast<FileSize>(_buffer.size));

                if (vfs_read_cnt != 0)
                {
                    last_immediate_read_cnt = vfs_read_cnt;
                }

                --immediate_read_cnt;
            }

            if (!isEOF && (read_size != requested_size))
            {
                _buffer_fetch_size = vfs::readFile(_hdl, {reinterpret_cast<u8 *>(_buffer.data), _buffer.size});

                auto const buffered_read_cnt = minValue(_buffer_fetch_size, requested_size - read_size);
                read_size += buffered_read_cnt;

                if (0 != buffered_read_cnt)
                {
                    memcpy(buffer.data(), reinterpret_cast<u8 const *>(_buffer.data) + _buffer_curr_offset,
                           integral_cast<usize>(buffered_read_cnt));
                }

                _buffer_curr_offset += buffered_read_cnt;
            }
            else if (last_immediate_read_cnt != 0)
            {
                memcpy(_buffer.data, reinterpret_cast<u8 const *>(buffer.data()) + read_size - last_immediate_read_cnt,
                       integral_cast<usize>(last_immediate_read_cnt));
                _buffer_fetch_size = last_immediate_read_cnt;
                _buffer_curr_offset = last_immediate_read_cnt;
            }

            return read_size;
        }
        else
        {
            return vfs::readFile(_hdl, buffer, cnt);
        }
    }

    return 0;
}

sb::FileSize sb::FileInputStream::getLength()
{
    if (sb_expect(_hdl.isValid()) && (-1 == _cached_file_length))
    {
        _cached_file_length = vfs::getFileLength(_hdl);
    }

    return (_cached_file_length == -1) ? 0 : _cached_file_length;
}

void sb::FileInputStream::reset(FileHdl hdl)
{
    if (hdl != _hdl)
    {
        if (_hdl.isValid())
        {
            vfs::closeFile(_hdl);
        }

        _hdl = hdl;
    }
}

sb::FileHdl sb::FileInputStream::swap(FileHdl hdl)
{
    FileHdl const prev_hdl = _hdl;
    _hdl = hdl;
    return prev_hdl;
}
