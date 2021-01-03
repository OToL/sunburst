#include <sb_core/hash/crc.h>

#include <sb_std/iterator>

#include <sb_core/extern_prolog.h>
#include <catch2/catch.hpp>

using namespace sb;

static u8 const CRC64_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static u64 const CRC64_ARRAY_TEST_HASH = 0x674d16005b262152ULL;

static char const * const CRC64_STR_TEST = "Hello World !";
static u64 const CRC64_STR_TEST_HASH = 0x69172790b5f8880fULL;

TEST_CASE("Null CRC64", "[crc64]")
{
    REQUIRE(0ULL == computeCRC64(sbstd::span<u8>{}));
    REQUIRE(0ULL == computeCRC64({reinterpret_cast<u8 const *>("Hello"), 0}));
    REQUIRE(0ULL == computeCRC64(""));
}

TEST_CASE("Raw buffer CRC64 hash", "[crc64]")
{
    u64 const test_crc = computeCRC64(CRC64_ARRAY_TEST);

    REQUIRE(CRC64_ARRAY_TEST_HASH == test_crc);
}

TEST_CASE("String CRC64 hash", "[crc64]")
{
    u64 const test_crc = computeCRC64(CRC64_STR_TEST);

    REQUIRE(CRC64_STR_TEST_HASH == test_crc);
}

#include <sb_core/extern_epilog.h>
