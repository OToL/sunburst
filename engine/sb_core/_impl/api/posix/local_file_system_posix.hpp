#include <sb_core/core.h>
#include <sb_core/string/string_format.h>
#include <sb_core/error/error.h>
#include <sb_core/conversion.h>
#include <sb_core/io/io.h>
#include <sb_core/string/fix_string.h>
#include <sb_core/_impl/io/local_file_system.h>

#include <sb_std/cstdio>

sb::internal::LayerFileHdl sb::internal::platformOpenFileRead(char const * path, FileFormat fmt)
{
    FixString<3> flags{"r"};

    if (fmt == FileFormat::BIN)
    {
        flags.push_back('b');
    }

    FILE * hdl = nullptr;
    auto const res = fopen_s(&hdl, path, flags.data());

    if (sbExpect(0 == res))
    {
        return {(void *)hdl};
    }

    return {};
}

sb::internal::LayerFileHdl sb::internal::platformOpenFileReadWrite(char const * path, FileWriteMode mode,
                                                                   FileFormat fmt)
{
    FixString<5> flags;

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

    if (fmt == FileFormat::BIN)
    {
        flags.push_back('b');
    }

    flags.push_back('x');

    FILE * hdl = nullptr;
    auto const res = fopen_s(&hdl, path, flags.data());

    if (sbExpect(0 == res))
    {
        return {(void *)hdl};
    }

    return {};
}

sb::internal::LayerFileHdl sb::internal::platformOpenFileWrite(char const * path, FileWriteMode mode, FileFormat fmt)
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
        sbWarn(false, "Unhandled write mode {}", getEnumValue(mode));
    }

    if (fmt == FileFormat::BIN)
    {
        flags.push_back('b');
    }

    flags.push_back('x');

    FILE * hdl = nullptr;
    auto const res = fopen_s(&hdl, path, flags.data());

    if (sbExpect(0 == res))
    {
        return {(void *)hdl};
    }

    return {};
}

sb::internal::LayerFileHdl sb::internal::platformCreateFileWrite(char const * path, FileFormat fmt)
{
    FixString<3> flags{"w"};

    if (fmt == FileFormat::BIN)
    {
        flags.push_back('b');
    }

    FILE * hdl = nullptr;
    auto const res = fopen_s(&hdl, path, flags.data());

    if (sbExpect(0 == res))
    {
        return {(void *)hdl};
    }

    return {};
}

sb::internal::LayerFileHdl sb::internal::platformCreateFileReadWrite(char const * path, FileFormat fmt)
{
    FixString<3> flags{"w+"};

    if (fmt == FileFormat::BIN)
    {
        flags.push_back('b');
    }

    FILE * hdl = nullptr;
    auto const res = fopen_s(&hdl, path, flags.data());

    if (sbExpect(0 == res))
    {
        return {(void *)hdl};
    }

    return {};
}

void sb::internal::platformCloseFile(LayerFileHdl hdl)
{
    sbAssert(nullptr != hdl.value);

    fclose(reinterpret_cast<FILE *>(hdl.value));
}

sb::FileSize sb::internal::platformReadFile(LayerFileHdl hdl, u8 * buffer, FileSize count)
{
    sbAssert(nullptr != hdl.value);

    return numericConv<FileSize>(fread((void *)buffer, 1, (usize)count, reinterpret_cast<FILE *>(hdl.value)));
}

sb::FileSize sb::internal::platformWriteFile(LayerFileHdl hdl, u8 const * buffer, FileSize count)
{
    sbAssert(nullptr != hdl.value);

    return numericConv<FileSize>(fwrite((void *)buffer, 1, (usize)count, reinterpret_cast<FILE *>(hdl.value)));
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
