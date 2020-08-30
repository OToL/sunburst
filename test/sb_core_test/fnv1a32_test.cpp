#include <sb_core/hash/fnv1a.h>

#include <sb_std/iterator>
#include <sb_std/xutility>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

static ui8 const FNV32_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static ui32 const FNV32_ARRAY_TEST_HASH = 0x644280dfU;

static char const * const FNV32_STR_TEST = "Hello World !";
static ui32 const FNV32_STR_TEST_HASH = 0xcad2b6acU;

TEST_CASE("Null FNV32", "[fnv1a32]")
{
    REQUIRE(0U == computeFNV1a32(sbstd::span<ui8>{}));
    REQUIRE(0U == computeFNV1a32({sbstd::data(FNV32_ARRAY_TEST), 0}));
    REQUIRE(0U == computeFNV1a32(""));
}

TEST_CASE("Raw buffer FNV32 hash", "[fnv1a32]")
{
    ui32 const fnv_val = computeFNV1a32(FNV32_ARRAY_TEST);

    REQUIRE(FNV32_ARRAY_TEST_HASH == fnv_val);
}

TEST_CASE("String FNV32 hash", "[fnv1a32]")
{
    ui32 const fnv_val = computeFNV1a32(FNV32_STR_TEST);

    REQUIRE(FNV32_STR_TEST_HASH == fnv_val);
}

#include <catch2/test_epilog.h>
