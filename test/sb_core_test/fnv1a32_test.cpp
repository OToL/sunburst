#include <sb_core/hash/fnv1a.h>

#include <sb_std/iterator>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

static ui8 const FNV32_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static FNV1a32::Value const FNV32_ARRAY_TEST_HASH = 0x644280dfU;

static char const * const FNV32_STR_TEST = "Hello World !";
static FNV1a32::Value const FNV32_STR_TEST_HASH = 0xcad2b6acU;

TEST_CASE("Null FNV32", "[fnv1a32]")
{
    REQUIRE(FNV1a32::OFFSET == FNV1a32::compute(nullptr, 0));
    REQUIRE(FNV1a32::OFFSET == FNV1a32::compute((ui8 const * const) "Hello", 0));
    REQUIRE(FNV1a32::OFFSET == FNV1a32::compute(""));
}

TEST_CASE("Raw buffer FNV32 hash", "[fnv1a32]")
{
    FNV1a32::Value const fnv_val = FNV1a32::compute(FNV32_ARRAY_TEST, sbstd::size(FNV32_ARRAY_TEST));

    REQUIRE(FNV32_ARRAY_TEST_HASH == fnv_val);
}

TEST_CASE("String FNV32 hash", "[fnv1a32]")
{
    FNV1a32::Value const fnv_val = FNV1a32::compute(FNV32_STR_TEST);

    REQUIRE(FNV32_STR_TEST_HASH == fnv_val);
}

#include <catch2/test_epilog.h>
