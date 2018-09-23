#include <core/hash/crc.h>
#include <libc++/iterator>

#include <gtest/gtest_common.h>

namespace sb { namespace test {

static ui8 const CRC64_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static CRC64Value const CRC64_ARRAY_TEST_HASH = 0x674d16005b262152ULL;

static char const * const CRC64_STR_TEST = "Hello World !";
static CRC64Value const CRC64_STR_TEST_HASH = 0x69172790b5f8880fULL;

TEST(CRC64, NullCRC)
{
    EXPECT_EQ(CRC64_NULL, computeCRC64(nullptr, 0));
    EXPECT_EQ(CRC64_NULL, computeCRC64((ui8 const * const) "Hello", 0));
    EXPECT_EQ(CRC64_NULL, computeCRC32(""));
}

TEST(CRC64, ComputeBuffer)
{
    CRC64Value const test_crc = computeCRC64(CRC64_ARRAY_TEST, wstd::size(CRC64_ARRAY_TEST));

    EXPECT_EQ(CRC64_ARRAY_TEST_HASH, test_crc);
}

TEST(CRC64, ComputeStr)
{
    CRC64Value const test_crc = computeCRC64(CRC64_STR_TEST);

    EXPECT_EQ(CRC64_STR_TEST_HASH, test_crc);
}

}} // namespace sb::test
