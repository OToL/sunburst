#include <sb_core/io/path.h>
#include <sb_core/conversion.h>
#include <sb_core/error.h>
#include <sb_core/string/utility.h>
#include <sb_core/core.h>

#include <sb_std/algorithm>

constexpr char const * INVALID_LOGICAL_PATH_SEPARATOR = "\\";

sb::b8 sb::isVFSPathValid(char const * vfs_path)
{
    sbAssert(nullptr != vfs_path);

    return ((VFS_PATH_SEPARATOR == *vfs_path) && (nullptr == strchr(vfs_path, *INVALID_LOGICAL_PATH_SEPARATOR)));    
}

char * sb::concatLocalPath(sbstd::span<char> base_path, usize base_path_len, char const * path_cat)
{
    sbAssert(base_path[base_path_len] == 0);
    usize concat_offset = base_path_len;

    if (base_path[concat_offset - 1] != LOCAL_PATH_SEPARATOR)
    {
        if (sbExpect(base_path_len < base_path.size()))
        {
            base_path[concat_offset] = LOCAL_PATH_SEPARATOR;
            ++concat_offset;
        }
        else
        {
            return base_path.data();
        }
    }

    strCpyT(base_path.data() + concat_offset,
            numericConv<usize>(base_path.size() - concat_offset), path_cat);

    return base_path.data();
}

char * sb::concatLocalPath(sbstd::span<char> base_path, char const * path_cat)
{
    auto const base_path_len = strlen(base_path.data());

    return concatLocalPath(base_path, base_path_len, path_cat);
}

char * sb::normalizeLocalPath(char * path)
{
    sbAssert(nullptr != path);
    return path;
}
