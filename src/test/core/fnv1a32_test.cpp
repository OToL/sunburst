#include <core/hash/fnv1a.h>
#include <libc++/iterator>

#include <gtest/gtest_common.h>

namespace sb { namespace test {

static ui8 const FNV32_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static FNV1a32::Value const FNV32_ARRAY_TEST_HASH = 0x644280dfU;

static char const * const FNV32_STR_TEST = "Hello World !";
static FNV1a32::Value const FNV32_STR_TEST_HASH = 0xcad2b6acU;

TEST(FNV1A32, NullFNV)
{
    EXPECT_EQ(FNV1a32::OFFSET, FNV1a32::compute(nullptr, 0));
    EXPECT_EQ(FNV1a32::OFFSET, FNV1a32::compute((ui8 const * const) "Hello", 0));
    EXPECT_EQ(FNV1a32::OFFSET, FNV1a32::compute(""));
}

TEST(FNV1A32, ComputeBuffer)
{
    FNV1a32::Value const fnv_val = FNV1a32::compute(FNV32_ARRAY_TEST, wstd::size(FNV32_ARRAY_TEST));

    EXPECT_EQ(FNV32_ARRAY_TEST_HASH, fnv_val);
}

TEST(FNV1A32, ComputeStr)
{
    FNV1a32::Value const fnv_val = FNV1a32::compute(FNV32_STR_TEST);

    EXPECT_EQ(FNV32_STR_TEST_HASH, fnv_val);
}

}} // namespace sb::test