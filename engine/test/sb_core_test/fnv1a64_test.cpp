#include <sb_core/hash/fnv1a.h>
#include <sb_slw/iterator>

#include <sb_slw/xutility>
#include <sb_slw/iterator>

#include <extern_prolog.h>
#include <catch2/catch.hpp>
#include <extern_epilog.h>

using namespace sb;

static u8 const FNV64_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static u64 const FNV64_ARRAY_TEST_HASH = 0x23ae0f3c3e4e7adfULL;

static char const * const FNV64_STR_TEST = "Hello World !";
static u64 const FNV64_STR_TEST_HASH = 0x9b90b3ce8e37ea0cULL;

TEST_CASE("Null FNV64", "[fnv64]")
{
    REQUIRE(0ULL == computeFNV1a64(slw::span<u8>{}));
    REQUIRE(0ULL == computeFNV1a64({slw::data(FNV64_ARRAY_TEST), 0}));
    REQUIRE(0ULL == computeFNV1a64(""));
}

TEST_CASE("Raw buffer FNV64 hash", "[fnv64]")
{
    u64 const fnv_val = computeFNV1a64(FNV64_ARRAY_TEST);

    REQUIRE(FNV64_ARRAY_TEST_HASH == fnv_val);
}

TEST_CASE("String FNV64 hash", "[fnv64]")
{
    u64 const fnv_val = computeFNV1a64(FNV64_STR_TEST);

    REQUIRE(FNV64_STR_TEST_HASH == fnv_val);
}

