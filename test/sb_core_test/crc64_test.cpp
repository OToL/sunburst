#include <sb_core/hash/crc.h>

#include <sb_std/iterator>

#include <catch2/xcatch.hpp>
#include <catch2/test_prolog.h>

using namespace sb;

static ui8 const CRC64_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static CRC64::Value const CRC64_ARRAY_TEST_HASH = 0x674d16005b262152ULL;

static char const * const CRC64_STR_TEST = "Hello World !";
static CRC64::Value const CRC64_STR_TEST_HASH = 0x69172790b5f8880fULL;

TEST_CASE("Null CRC64", "[crc64]")
{
    REQUIRE(CRC64::NULL_VALUE == CRC64::compute(nullptr, 0));
    REQUIRE(CRC64::NULL_VALUE == CRC64::compute((ui8 const * const) "Hello", 0));
    REQUIRE(CRC64::NULL_VALUE == CRC32::compute(""));
}

TEST_CASE("Raw buffer CRC64 hash", "[crc64]")
{
    CRC64::Value const test_crc = CRC64::compute(CRC64_ARRAY_TEST, sbstd::size(CRC64_ARRAY_TEST));

    REQUIRE(CRC64_ARRAY_TEST_HASH == test_crc);
}

TEST_CASE("String CRC64 hash", "[crc64]")
{
    CRC64::Value const test_crc = CRC64::compute(CRC64_STR_TEST);

    REQUIRE(CRC64_STR_TEST_HASH == test_crc);
}

#include <catch2/test_epilog.h>
