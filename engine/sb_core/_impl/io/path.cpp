#include <sb_core/io/path.h>
#include <sb_core/io/file_system.h>
#include <sb_core/conversion.h>
#include <sb_core/error.h>
#include <sb_core/string/utility.h>
#include <sb_core/core.h>

constexpr char const * INVALID_LOGICAL_PATH_SEPARATOR = "\\";

sb::b8 sb::isVfsPathValid(char const * vfs_path)
{
    sbAssert(nullptr != vfs_path);

    return ((VFS_PATH_SEPARATOR == *vfs_path) &&
            (nullptr == strchr(vfs_path, *INVALID_LOGICAL_PATH_SEPARATOR)));
}

char * sb::concatPhysPath(sbstd::span<char> base_path, char const * path_cat)
{
    auto const base_path_capacity = base_path.size();
    auto base_path_len = strlen(base_path.data());
    auto const cat_len = strlen(path_cat);
    auto expected_len = base_path_len + cat_len;

    if (base_path[base_path_len - 1] != PHYS_PATH_SEPARATOR)
    {
        ++expected_len;
    }

    if (sbExpect(base_path_capacity > expected_len))
    {
        if (base_path[base_path_len - 1] != PHYS_PATH_SEPARATOR)
        {
            ++base_path_len;
            base_path[base_path_len] = PHYS_PATH_SEPARATOR;
        }

        strCpyT(base_path.data() + base_path_len,
                numericConv<usize>(base_path_capacity - base_path_len), path_cat);
    }

    return base_path.data();
}
