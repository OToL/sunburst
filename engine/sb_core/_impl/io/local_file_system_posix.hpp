#include <sb_core/core.h>
#include <sb_core/string/format.h>
#include <sb_core/error/error.h>
#include <sb_core/cast.h>
#include <sb_core/io/io.h>
#include <sb_core/string/fix_string.h>
#include <sb_core/_impl/io/local_file_system.h>

#include <sb_std/cstdio>

sb::internal::LayerFile sb::internal::platformOpenFileRead(char const * path, FileFormat fmt)
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

sb::internal::LayerFile sb::internal::platformOpenFileReadWrite(char const * path, FileWriteMode mode, FileFormat fmt)
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
        sb_warn(false, "Unhandled write mode {}", integral_cast<>(mode));
    }

    if (fmt == FileFormat::BIN)
    {
        flags.push_back('b');
    }

    flags.push_back('x');

    FILE * hdl = nullptr;
    auto const res = fopen_s(&hdl, path, flags.data());

    if (sb_expect(0 == res))
    {
        return {(void *)hdl};
    }

    return {};
}

sb::internal::LayerFile sb::internal::platformOpenFileWrite(char const * path, FileWriteMode mode, FileFormat fmt)
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

    flags.push_back('x');

    FILE * hdl = nullptr;
    auto const res = fopen_s(&hdl, path, flags.data());

    if (sb_expect(0 == res))
    {
        return {(void *)hdl};
    }

    return {};
}

sb::internal::LayerFile sb::internal::platformCreateFileWrite(char const * path, FileFormat fmt)
{
    FixString<3> flags{"w"};

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

sb::internal::LayerFile sb::internal::platformCreateFileReadWrite(char const * path, FileFormat fmt)
{
    FixString<3> flags{"w+"};

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

void sb::internal::platformCloseFile(LayerFile hdl)
{
    sb_assert(nullptr != hdl.value);

    fclose(reinterpret_cast<FILE *>(hdl.value));
}

sb::FileSize sb::internal::platformReadFile(LayerFile hdl, u8 * buffer, FileSize count)
{
    sb_assert(nullptr != hdl.value);

    return integral_cast<FileSize>(fread((void *)buffer, 1, (usize)count, reinterpret_cast<FILE *>(hdl.value)));
}

sb::FileSize sb::internal::platformWriteFile(LayerFile hdl, u8 const * buffer, FileSize count)
{
    sb_assert(nullptr != hdl.value);

    return integral_cast<FileSize>(fwrite((void *)buffer, 1, (usize)count, reinterpret_cast<FILE *>(hdl.value)));
}

// @todo: could be optimized
sb::FileSize sb::internal::platformFileLength(LayerFile hdl)
{
    sb_assert(nullptr != hdl.value);

    FILE * const actual_hdl = reinterpret_cast<FILE *>(hdl.value);

    auto const curr_pos = ftell(actual_hdl);

    if (sb_dont_expect(0 != fseek(actual_hdl, 0, SEEK_END)))
    {
        return 0;
    }

    auto const file_len = ftell(actual_hdl);

    [[maybe_unused]] int const res = fseek(actual_hdl, curr_pos, SEEK_SET);
    sb_assert(res == 0);

    return file_len;
}
