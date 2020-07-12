#include <core/hash/crc.h>

#include <wstd/iterator>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

static ui8 const CRC32_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static CRC32::Value const CRC32_ARRAY_TEST_HASH = 0x5935734fU;

static char const * const CRC32_STR_TEST = "Hello World !";
static CRC32::Value const CRC32_STR_TEST_HASH = 0xc7c7535eU;

TEST_CASE("Null CRC32", "[crc32]")
{
    REQUIRE(CRC32::NULL_VALUE == CRC32::compute(nullptr, 0));
    REQUIRE(CRC32::NULL_VALUE == CRC32::compute((ui8 const * const) "Hello", 0));
    REQUIRE(CRC32::NULL_VALUE == CRC32::compute(""));
}

TEST_CASE("Raw buffer CRC32 hash", "[crc32]")
{
    CRC32::Value const test_crc = CRC32::compute(CRC32_ARRAY_TEST, wstd::size(CRC32_ARRAY_TEST));

    REQUIRE(CRC32_ARRAY_TEST_HASH == test_crc);
}

TEST_CASE("String CRC32 hash", "[crc32]")
{
    CRC32::Value const test_crc = CRC32::compute(CRC32_STR_TEST);

    REQUIRE(CRC32_STR_TEST_HASH == test_crc);
}

#include <catch2/test_epilog.h>
