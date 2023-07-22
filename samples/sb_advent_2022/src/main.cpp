#include "advent.h"

#include <sb_core/core.h>
#include <sb_core/error.h>
#include <sb_core/log.h>
#include <sb_core/io/virtual_file_system.h>

int main()
{
    using namespace sb;

    vfs::initialize();

    vfs::MountResult const mount_res =
        vfs::mountLocalFileSystem("/data", R"(c:\Users\barch\Documents\dev\sunburst\samples\sb_advent_2022\data\)");
    sb_assert(mount_res.success);

    advent::runDay22();

    vfs::terminate();

    return 0;
}
