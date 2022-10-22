#include <sb_core/hash/fnv1a.h>

#include <sb_slw/iterator>
#include <sb_slw/xutility>

#include <extern_prolog.h>
#include <catch2/catch.hpp>

using namespace sb;

static u8 const FNV32_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static u32 const FNV32_ARRAY_TEST_HASH = 0x644280dfU;

static char const * const FNV32_STR_TEST = "Hello World !";
static u32 const FNV32_STR_TEST_HASH = 0xcad2b6acU;

TEST_CASE("Null FNV32", "[fnv1a32]")
{
    REQUIRE(0U == computeFNV1a32(slw::span<u8>{}));
    REQUIRE(0U == computeFNV1a32({slw::data(FNV32_ARRAY_TEST), 0}));
    REQUIRE(0U == computeFNV1a32(""));
}

TEST_CASE("Raw buffer FNV32 hash", "[fnv1a32]")
{
    u32 const fnv_val = computeFNV1a32(FNV32_ARRAY_TEST);

    REQUIRE(FNV32_ARRAY_TEST_HASH == fnv_val);
}

TEST_CASE("String FNV32 hash", "[fnv1a32]")
{
    u32 const fnv_val = computeFNV1a32(FNV32_STR_TEST);

    REQUIRE(FNV32_STR_TEST_HASH == fnv_val);
}

#include <extern_epilog.h>
