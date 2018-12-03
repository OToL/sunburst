#include <core/io/path.h>
#include <core/io/file_system.h>
#include <core/conversion.h>
#include <core/error.h>
#include <core/utility.h>
#include <core/platform.h>

namespace sb {

constexpr char const * INVALID_LOGICAL_PATH_SEPARATOR = "\\";

b8 isLogicalPathValid(char const * path)
{
    sbAssert(nullptr != path);

    return ((*LOGICAL_PATH_SEPARATOR == *path) && (nullptr == strchr(path, *INVALID_LOGICAL_PATH_SEPARATOR)));
}

char * concatPhysicalPaths(wstd::span<char> base_path, char const * path_cat)
{
    auto const base_path_capacity = base_path.size();
    auto base_path_len = numericCast<sptrdiff>(strlen(base_path.data()));
    auto const cat_len = numericCast<sptrdiff>(strlen(path_cat));
    auto expected_len = base_path_len + cat_len;

    if (base_path[base_path_len - 1] != *PHYSICAL_PATH_SEPARATOR)
    {
        ++expected_len;
    }

    if (sbExpectTrue(base_path_capacity > expected_len))
    {
        if (base_path[base_path_len - 1] != *PHYSICAL_PATH_SEPARATOR)
        {
            ++base_path_len;
            base_path[base_path_len] = *PHYSICAL_PATH_SEPARATOR;
        }

        strCpyT(base_path.data() + base_path_len, numericCast<usize>(base_path_capacity - base_path_len), path_cat);
    }

    return base_path.data();
}

} // namespace sb
