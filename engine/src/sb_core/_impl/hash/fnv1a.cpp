#include <sb_core/hash/fnv1a.h>
#include <sb_core/core.h>

#include <sb_slw/span>
#include <sb_slw/string_view>

sb::u64 sb::computeFNV1a64(slw::span_fw<u8 const> const & buffer)
{
    return computeFNV1a64(buffer.data(), buffer.size());
}

sb::u64 sb::computeFNV1a64(slw::string_view_fw const & buffer)
{
    return computeFNV1a64(buffer.data(), buffer.size());
}

sb::u32 sb::computeFNV1a32(slw::span_fw<u8 const> const & buffer)
{
    return computeFNV1a32(buffer.data(), buffer.size());
}

sb::u32 sb::computeFNV1a32(slw::string_view_fw const & buffer)
{
    return computeFNV1a32(buffer.data(), buffer.size());
}
