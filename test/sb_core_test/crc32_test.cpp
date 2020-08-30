#include <sb_core/hash/crc.h>

#include <sb_std/iterator>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

static ui8 const CRC32_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static ui32 const CRC32_ARRAY_TEST_HASH = 0x5935734fU;

static char const * const CRC32_STR_TEST = "Hello World !";
static ui32 const CRC32_STR_TEST_HASH = 0xc7c7535eU;

TEST_CASE("Null CRC32", "[crc32]")
{
    REQUIRE(0U == computeCRC32(sbstd::span<ui8>{}));
    REQUIRE(0U == computeCRC32({(ui8 const *) "Hello", 0}));
    REQUIRE(0U == computeCRC32(""));
}

TEST_CASE("Raw buffer CRC32 hash", "[crc32]")
{
    ui32 const test_crc = computeCRC32(CRC32_ARRAY_TEST);

    REQUIRE(CRC32_ARRAY_TEST_HASH == test_crc);
}

TEST_CASE("String CRC32 hash", "[crc32]")
{
    ui32 const test_crc = computeCRC32(CRC32_STR_TEST);

    REQUIRE(CRC32_STR_TEST_HASH == test_crc);
}

#include <catch2/test_epilog.h>
