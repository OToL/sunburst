#include <core/hash/fnv1a.h>
#include <wstd/iterator>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

static ui8 const FNV64_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static FNV1a64::Value const FNV64_ARRAY_TEST_HASH = 0x23ae0f3c3e4e7adfULL;

static char const * const FNV64_STR_TEST = "Hello World !";
static FNV1a64::Value const FNV64_STR_TEST_HASH = 0x9b90b3ce8e37ea0cULL;

TEST_CASE("Null FNV64", "[fnv64]")
{
    REQUIRE(FNV1a64::OFFSET == FNV1a64::compute(nullptr, 0));
    REQUIRE(FNV1a64::OFFSET == FNV1a64::compute((ui8 const * const) "Hello", 0));
    REQUIRE(FNV1a64::OFFSET == FNV1a64::compute(""));
}

TEST_CASE("Raw buffer FNV64 hash", "[fnv64]")
{
    FNV1a64::Value const fnv_val = FNV1a64::compute(FNV64_ARRAY_TEST, wstd::size(FNV64_ARRAY_TEST));

    REQUIRE(FNV64_ARRAY_TEST_HASH == fnv_val);
}

TEST_CASE("String FNV64 hash", "[fnv64]")
{
    FNV1a64::Value const fnv_val = FNV1a64::compute(FNV64_STR_TEST);

    REQUIRE(FNV64_STR_TEST_HASH == fnv_val);
}

#include <catch2/test_epilog.h>
