#include <sb_core/memory/utility.h>
#include <sb_core/os.h>

#include <sb_slw/cstdlib>
#include <sb_slw/cstring>

void sb::zeroMemory(void * const memPtr, usize const byteCount)
{
    slw::memset(memPtr, 0, byteCount);
}

[[noreturn]] void sb::notifyOOM([[maybe_unused]] usize requestedSize, char const * message)
{
    outputDebugString(message);

    slw::exit(-1);
}
