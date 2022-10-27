#include <sb_core/hash/crc.h>

#include <sb_slw/iterator>

#include <extern_prolog.h>
#include <catch2/catch.hpp>
#include <extern_epilog.h>

using namespace sb;

static u8 const CRC32_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static u32 const CRC32_ARRAY_TEST_HASH = 0x5935734fU;

static char const * const CRC32_STR_TEST = "Hello World !";
static u32 const CRC32_STR_TEST_HASH = 0xc7c7535eU;

TEST_CASE("Null CRC32", "[crc32]")
{
    REQUIRE(0U == computeCRC32(slw::span<u8>{}));
    REQUIRE(0U == computeCRC32({reinterpret_cast<u8 const *>("Hello"), 0}));
    REQUIRE(0U == computeCRC32(""));
}

TEST_CASE("Raw buffer CRC32 hash", "[crc32]")
{
    u32 const test_crc = computeCRC32(CRC32_ARRAY_TEST);

    REQUIRE(CRC32_ARRAY_TEST_HASH == test_crc);
}

TEST_CASE("String CRC32 hash", "[crc32]")
{
    u32 const test_crc = computeCRC32(CRC32_STR_TEST);

    REQUIRE(CRC32_STR_TEST_HASH == test_crc);
}

