#include <core/io/path.h>
#include <core/io/file_system.h>
#include <core/conversion.h>
#include <core/error.h>
#include <core/string/utility.h>
#include <base/base.h>

namespace sb {

constexpr char const * INVALID_LOGICAL_PATH_SEPARATOR = "\\";

b8 LogicPath::isValid(char const * path)
{
    sbAssert(nullptr != path);

    return ((*SEPARATOR == *path) && (nullptr == strchr(path, *INVALID_LOGICAL_PATH_SEPARATOR)));
}

char * PhysicPath::concat(wstd::span<char> base_path, char const * path_cat)
{
    auto const base_path_capacity = base_path.size();
    auto base_path_len = strlen(base_path.data());
    auto const cat_len = strlen(path_cat);
    auto expected_len = base_path_len + cat_len;

    if (base_path[base_path_len - 1] != *PPath::SEPARATOR)
    {
        ++expected_len;
    }

    if (sbExpect(base_path_capacity > expected_len))
    {
        if (base_path[base_path_len - 1] != *PPath::SEPARATOR)
        {
            ++base_path_len;
            base_path[base_path_len] = *PPath::SEPARATOR;
        }

        strCpyT(base_path.data() + base_path_len,
                numericConv<usize>(base_path_capacity - base_path_len), path_cat);
    }

    return base_path.data();
}

} // namespace sb
