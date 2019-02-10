#include <core/string/string_format.h>
#include <core/error.h>
#include <core/platform.h>
#include <core/conversion.h>
#include <core/io/base.h>
#include <core/string/static_string.h>
#include <core/enum.h>
#include <core/_impl/io/file_system_platform.h>

#include <libc++/cstdio>

namespace sb {

PlatformFileHdl platformOpenFileRead(char const * path, FileFormat fmt)
{
    StaticString<3> flags{"r"};

    if (fmt == FileFormat::Bin)
    {
        flags.push_back('b');
    }

    return {(void *)fopen(path, flags.data())};
}

PlatformFileHdl platformOpenFileWrite(char const * path, FileWriteMode mode, FileFormat fmt)
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

PlatformFileHdl platformCreateFile(char const * path, FileFormat fmt)
{
    StaticString<3> flags{"w"};

    if (fmt == FileFormat::Bin)
    {
        flags.push_back('b');
    }

    return {(void *)fopen(path, flags.c_str())};
}

void platformCloseFile(PlatformFileHdl hdl)
{
    sbAssert(nullptr != hdl.m_value);

    fclose((FILE *)hdl.m_value);
}

FileSize platformReadFile(PlatformFileHdl hdl, ui8 * buffer, FileSize count)
{
    sbAssert(nullptr != hdl.m_value);

    return numericCast<FileSize>(fread((void *)buffer, 1, (usize)count, (FILE *)hdl.m_value));
}

FileSize platformFileLength(PlatformFileHdl hdl)
{
    sbAssert(nullptr != hdl.m_value);

    FILE * const actual_hdl = reinterpret_cast<FILE *>(hdl.m_value);

    auto const curr_pos = ftell(actual_hdl);

    if (sbExpectFalse(0 != fseek(actual_hdl, 0, SEEK_END)))
    {
        return 0;
    }

    auto const file_len = ftell(actual_hdl);

    [[maybe_unused]] int const res = fseek(actual_hdl, curr_pos, SEEK_SET);
    sbAssert(res == 0);

    return file_len;
}

} // namespace sb
