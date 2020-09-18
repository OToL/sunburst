#include <sb_core/memory/memory.h>
#include <sb_core/os.h>

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
