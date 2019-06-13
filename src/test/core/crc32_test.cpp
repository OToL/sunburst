#include <core/platform.h>
#include <core/hash/crc.h>
#include <libc++/iterator>

#include <gtest/gtest_common.h>

namespace sb { namespace test {

static ui8 const CRC32_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static CRC32Value const CRC32_ARRAY_TEST_HASH = 0x5935734fU;

static char const * const CRC32_STR_TEST = "Hello World !";
static CRC32Value const CRC32_STR_TEST_HASH = 0xc7c7535eU;

TEST(CRC32, NullCRC)
{
    EXPECT_EQ(CRC32_NULL, computeCRC32(nullptr, 0));
    EXPECT_EQ(CRC32_NULL, computeCRC32((ui8 const * const) "Hello", 0));
    EXPECT_EQ(CRC32_NULL, computeCRC32(""));
}

TEST(CRC32, ComputeBuffer)
{
    CRC32Value const test_crc = computeCRC32(CRC32_ARRAY_TEST, wstd::size(CRC32_ARRAY_TEST));

    EXPECT_EQ(CRC32_ARRAY_TEST_HASH, test_crc);
}

TEST(CRC32, ComputeStr)
{
    CRC32Value const test_crc = computeCRC32(CRC32_STR_TEST);

    EXPECT_EQ(CRC32_STR_TEST_HASH, test_crc);
}

}} // namespace sb::test
