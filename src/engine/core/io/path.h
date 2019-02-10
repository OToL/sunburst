#pragma once

#include <core/platform.h>

#include <libc++/span>

namespace sb {

struct LogicPath
{
    static constexpr usize MAX_LEN = 255;
    static constexpr char const * SEPARATOR = "/";

    static b8 isValid(char const * path);

};

struct PhysicPath
{
    static constexpr usize MAX_LEN = 255;
    static constexpr char const * SEPARATOR = detail::PLATFORM_PATH_SEPARATOR;

    // base_path is modified & base_path.data() is returned
    static char * concat(wstd::span<char> base_path, char const * path_cat);
};

using LPath = LogicPath;
using PPath = PhysicPath;

} // namespace sb
