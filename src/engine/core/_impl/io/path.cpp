#include <core/io/path.h>
#include <core/io/file_system.h>
#include <core/conversion.h>
#include <core/error.h>

namespace sb {

constexpr char const * INVALID_LOGICAL_PATH_SEPARATOR = "\\";

ui8 isLogicalPathValid(char const * path)
{
    sbAssert(nullptr != path);

    return ((*LOGICAL_PATH_SEPARATOR == *path) && (nullptr == strchr(path, *INVALID_LOGICAL_PATH_SEPARATOR)));
}

} // namespace sb
