#include <core/hash/crc.h>
#include <libc++/iterator>

#include <gtest/gtest_common.h>

namespace sb { namespace test {

static ui8 const CRC64_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static CRC64::Value const CRC64_ARRAY_TEST_HASH = 0x674d16005b262152ULL;

static char const * const CRC64_STR_TEST = "Hello World !";
static CRC64::Value const CRC64_STR_TEST_HASH = 0x69172790b5f8880fULL;

TEST(CRC64, NullCRC)
{
    EXPECT_EQ(CRC64::NULL_VALUE, CRC64::compute(nullptr, 0));
    EXPECT_EQ(CRC64::NULL_VALUE, CRC64::compute((ui8 const * const) "Hello", 0));
    EXPECT_EQ(CRC64::NULL_VALUE, CRC32::compute(""));
}

TEST(CRC64, ComputeBuffer)
{
    CRC64::Value const test_crc = CRC64::compute(CRC64_ARRAY_TEST, wstd::size(CRC64_ARRAY_TEST));

    EXPECT_EQ(CRC64_ARRAY_TEST_HASH, test_crc);
}

TEST(CRC64, ComputeStr)
{
    CRC64::Value const test_crc = CRC64::compute(CRC64_STR_TEST);

    EXPECT_EQ(CRC64_STR_TEST_HASH, test_crc);
}

}} // namespace sb::test
