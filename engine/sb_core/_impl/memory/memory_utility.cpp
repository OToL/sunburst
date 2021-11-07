#include <sb_core/memory/utility.h>
#include <sb_core/system/system.h>

#include <sb_std/cstdlib>
#include <sb_std/cstring>

void sb::zeroMemory(void * const memPtr, usize const byteCount)
{
    sbstd::memset(memPtr, 0, byteCount);
}

[[noreturn]] void sb::notifyOOM([[maybe_unused]] usize requestedSize, char const * message)
{
    outputDebugString(message);

    sbstd::exit(-1);
}
