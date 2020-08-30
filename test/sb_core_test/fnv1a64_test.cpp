#include <sb_core/hash/fnv1a.h>
#include <sb_std/iterator>

#include <sb_std/xutility>
#include <sb_std/iterator>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

static ui8 const FNV64_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static ui64 const FNV64_ARRAY_TEST_HASH = 0x23ae0f3c3e4e7adfULL;

static char const * const FNV64_STR_TEST = "Hello World !";
static ui64 const FNV64_STR_TEST_HASH = 0x9b90b3ce8e37ea0cULL;

TEST_CASE("Null FNV64", "[fnv64]")
{
    REQUIRE(0ULL == computeFNV1a64(sbstd::span<ui8>{}));
    REQUIRE(0ULL == computeFNV1a64({sbstd::data(FNV64_ARRAY_TEST), 0}));
    REQUIRE(0ULL == computeFNV1a64(""));
}

TEST_CASE("Raw buffer FNV64 hash", "[fnv64]")
{
    ui64 const fnv_val = computeFNV1a64(FNV64_ARRAY_TEST);

    REQUIRE(FNV64_ARRAY_TEST_HASH == fnv_val);
}

TEST_CASE("String FNV64 hash", "[fnv64]")
{
    ui64 const fnv_val = computeFNV1a64(FNV64_STR_TEST);

    REQUIRE(FNV64_STR_TEST_HASH == fnv_val);
}

#include <catch2/test_epilog.h>