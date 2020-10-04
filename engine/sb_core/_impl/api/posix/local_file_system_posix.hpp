#include <sb_core/core.h>
#include <sb_core/string/string_format.h>
#include <sb_core/error.h>
#include <sb_core/conversion.h>
#include <sb_core/io/io.h>
#include <sb_core/string/static_string.h>
#include <sb_core/enum.h>
#include <sb_core/_impl/io/local_file_system.h>

#include <sb_std/cstdio>

sb::internal::LayerFileHdl sb::internal::platformOpenFileRead(char const * path, FileFormat fmt)
{
    StaticString<3> flags{"r"};

    if (fmt == FileFormat::Bin)
    {
        flags.push_back('b');
    }

    return {(void *)fopen(path, flags.data())};
}

sb::internal::LayerFileHdl sb::internal::platformOpenFileReadWrite(char const * path, FileWriteMode mode, FileFormat fmt)
{
    StaticString<5> flags;

    if (mode == FileWriteMode::APPEND)
    {
        flags.append("a+");
    }
    else if (mode == FileWriteMode::TRUNC)
    {
        flags.append("w+");
    }
    else
    {
        sbWarn(false, "Unhandled write mode {}", getEnumValue(mode));
    }

    if (fmt == FileFormat::Bin)
    {
        flags.push_back('b');
    }

    flags.push_back('x');

    return {(void *)fopen(path, flags.c_str())};
}

sb::internal::LayerFileHdl sb::internal::platformOpenFileWrite(char const * path, FileWriteMode mode, FileFormat fmt)
{
    StaticString<5> flags;

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
        sbWarn(false, "Unhandled write mode {}", getEnumValue(mode));
    }

    if (fmt == FileFormat::Bin)
    {
        flags.push_back('b');
    }

    flags.push_back('x');

    return {(void *)fopen(path, flags.c_str())};
}

sb::internal::LayerFileHdl sb::internal::platformCreateFileWrite(char const * path, FileFormat fmt)
{
    StaticString<3> flags{"w"};

    if (fmt == FileFormat::Bin)
    {
        flags.push_back('b');
    }

    return {(void *)fopen(path, flags.c_str())};
}

sb::internal::LayerFileHdl sb::internal::platformCreateFileReadWrite(char const * path, FileFormat fmt)
{
    StaticString<3> flags{"w+"};

    if (fmt == FileFormat::Bin)
    {
        flags.push_back('b');
    }

    return {(void *)fopen(path, flags.c_str())};
}

void sb::internal::platformCloseFile(LayerFileHdl hdl)
{
    sbAssert(nullptr != hdl.value);

    fclose((FILE *)hdl.value);
}

sb::FileSize sb::internal::platformReadFile(LayerFileHdl hdl, ui8 * buffer, FileSize count)
{
    sbAssert(nullptr != hdl.value);

    return numericConv<FileSize>(fread((void *)buffer, 1, (usize)count, (FILE *)hdl.value));
}

// @todo: could be optimized
sb::FileSize sb::internal::platformFileLength(LayerFileHdl hdl)
{
    sbAssert(nullptr != hdl.value);

    FILE * const actual_hdl = reinterpret_cast<FILE *>(hdl.value);

    auto const curr_pos = ftell(actual_hdl);

    if (sbDontExpect(0 != fseek(actual_hdl, 0, SEEK_END)))
    {
        return 0;
    }

    auto const file_len = ftell(actual_hdl);

    [[maybe_unused]] int const res = fseek(actual_hdl, curr_pos, SEEK_SET);
    sbAssert(res == 0);

    return file_len;
}
