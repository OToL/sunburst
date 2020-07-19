#include <core/base.h>

namespace sb {

static_assert(1 == sizeof(b8), "1 != sizeof(b8)");
static_assert(4 == sizeof(b32), "4 != sizeof(b32)");
static_assert(1 == sizeof(ui8), "1 != sizeof(ui8)");
static_assert(1 == sizeof(si8), "1 != sizeof(si8)");
static_assert(2 == sizeof(ui16), "2 != sizeof(ui16)");
static_assert(2 == sizeof(si16), "2 != sizeof(si16)");
static_assert(4 == sizeof(ui32), "4 != sizeof(ui32)");
static_assert(4 == sizeof(si32), "4 != sizeof(si32)");
static_assert(8 == sizeof(ui64), "8 != sizeof(ui64)");
static_assert(8 == sizeof(si64), "8 != sizeof(si64)");
static_assert(8 == sizeof(f64), "8 != sizeof(f64)");
static_assert(4 == sizeof(f32), "4 != sizeof(f32)");
static_assert(8 == sizeof(usize), "8 != sizeof(usize)");
static_assert(8 == sizeof(uptr), "8 != sizeof(uptr)");

} // namespace sb
