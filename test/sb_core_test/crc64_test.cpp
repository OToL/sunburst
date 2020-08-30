#include <sb_core/hash/crc.h>

#include <sb_std/iterator>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

static ui8 const CRC64_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static ui64 const CRC64_ARRAY_TEST_HASH = 0x674d16005b262152ULL;

static char const * const CRC64_STR_TEST = "Hello World !";
static ui64 const CRC64_STR_TEST_HASH = 0x69172790b5f8880fULL;

TEST_CASE("Null CRC64", "[crc64]")
{
    REQUIRE(0ULL == computeCRC64(sbstd::span<ui8>{}));
    REQUIRE(0ULL == computeCRC64({(ui8 const *) "Hello", 0}));
    REQUIRE(0ULL == computeCRC64(""));
}

TEST_CASE("Raw buffer CRC64 hash", "[crc64]")
{
    ui64 const test_crc = computeCRC64(CRC64_ARRAY_TEST);

    REQUIRE(CRC64_ARRAY_TEST_HASH == test_crc);
}

TEST_CASE("String CRC64 hash", "[crc64]")
{
    ui64 const test_crc = computeCRC64(CRC64_STR_TEST);

    REQUIRE(CRC64_STR_TEST_HASH == test_crc);
}

#include <catch2/test_epilog.h>
