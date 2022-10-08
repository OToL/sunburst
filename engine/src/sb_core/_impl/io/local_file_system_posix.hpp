#include "local_file_system.h"
#include <sb_core/core.h>
#include <sb_core/error.h>
#include <sb_core/cast.h>
#include <sb_core/io/io.h>
#include <sb_core/string/format.h>
#include <sb_core/string/fix_string.h>

#include <sb_std/cstdio>

sb::File sb::openLocalFileRead(char const * path, FileFormat fmt)
{
    FixString<3> flags{"r"};

    if (fmt == FileFormat::BIN)
    {
        flags.push_back('b');
    }

    FILE * hdl = nullptr;
    auto const res = fopen_s(&hdl, path, flags.data());

    if (sb_expect(0 == res))
    {
        return {(void *)hdl};
    }

    return {};
}

sb::File sb::openLocalFileWrite(char const * path, FileWriteMode mode, FileFormat fmt)
{
    FixString<5> flags;

    if (mode == FileWriteMode::APPEND)
    {
        flags.push_back('a');
    }
    else if (mode == FileWriteMode::TRUNC)
    {
        flags.push_back('w');
    }
    else
    {
        sb_warn(false, "Unhandled write mode {}", integral_cast<>(mode));
    }

    if (fmt == FileFormat::BIN)
    {
        flags.push_back('b');
    }

    FILE * hdl = nullptr;
    auto const res = fopen_s(&hdl, path, flags.data());

    if (sb_expect(0 == res))
    {
        return {(void *)hdl};
    }

    return {};
}

void sb::closeLocalFile(File hdl)
{
    sb_assert(nullptr != hdl.value);

    fclose(reinterpret_cast<FILE *>(hdl.value));
}

sb::FileSize sb::readLocalFile(File hdl, u8 * buffer, FileSize count)
{
    sb_assert(nullptr != hdl.value);

    return integral_cast<FileSize>(fread((void *)buffer, 1, (usize)count, reinterpret_cast<FILE *>(hdl.value)));
}

sb::FileSize sb::writeLocalFile(File hdl, u8 const * buffer, FileSize count)
{
    sb_assert(nullptr != hdl.value);

    return integral_cast<FileSize>(fwrite((void *)buffer, 1, (usize)count, reinterpret_cast<FILE *>(hdl.value)));
}

#if defined(SB_PLATFORM_WINDOWS)
#   define sb_ftelli64(hdl) _ftelli64(actual_hdl)
#   define sb_fseeki64(hdl, offset, mode) _fseeki64(actual_hdl, offset, mode)
#else
# error
#endif

// @todo: could be optimized
sb::FileSize sb::getLocalFileLength(File hdl)
{
    sb_assert(nullptr != hdl.value);

    FILE * const actual_hdl = reinterpret_cast<FILE *>(hdl.value);
    auto const curr_pos = sb_ftelli64(actual_hdl);

    if (sb_dont_expect(0 != sb_fseeki64(actual_hdl, 0, SEEK_END)))
    {
        return 0;
    }

    auto const file_len = sb_ftelli64(actual_hdl);

    [[maybe_unused]] int const res = sb_fseeki64(actual_hdl, curr_pos, SEEK_SET);
    sb_assert(res == 0);

    return file_len;
}

sb::b8 sb::isLocalFileValid(File hdl) 
{
    return (nullptr != hdl.value);
}
