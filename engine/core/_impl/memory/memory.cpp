#include <core/memory/memory.h>
#include <core/os.h>

#include <libc++/cstdlib>
#include <libc++/cstring>

namespace sb {

void zeroMemory(void * const memPtr, usize const byteCount)
{
    wstd::memset(memPtr, 0, byteCount);
}

[[noreturn]] void notifyOOM([[maybe_unused]] usize  requestedSize, char const * message)
{
    outputDebugString(message);

    wstd::exit(-1);
}

} // namespace sb
