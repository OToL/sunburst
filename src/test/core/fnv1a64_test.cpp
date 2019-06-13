#include <core/hash/fnv1a.h>
#include <libc++/iterator>

#include <gtest/gtest_common.h>

namespace sb { namespace test {

static ui8 const FNV64_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static FNV1a64::Value const FNV64_ARRAY_TEST_HASH = 0x23ae0f3c3e4e7adfULL;

static char const * const FNV64_STR_TEST = "Hello World !";
static FNV1a64::Value const FNV64_STR_TEST_HASH = 0x9b90b3ce8e37ea0cULL;

TEST(FNV64, NullFNV)
{
    EXPECT_EQ(FNV1a64::OFFSET, FNV1a64::compute(nullptr, 0));
    EXPECT_EQ(FNV1a64::OFFSET, FNV1a64::compute((ui8 const * const) "Hello", 0));
    EXPECT_EQ(FNV1a64::OFFSET, FNV1a64::compute(""));
}

TEST(FNV64, ComputeBuffer)
{
    FNV1a64::Value const fnv_val = FNV1a64::compute(FNV64_ARRAY_TEST, wstd::size(FNV64_ARRAY_TEST));

    EXPECT_EQ(FNV64_ARRAY_TEST_HASH, fnv_val);
}

TEST(FNV64, ComputeStr)
{
    FNV1a64::Value const fnv_val = FNV1a64::compute(FNV64_STR_TEST);

    EXPECT_EQ(FNV64_STR_TEST_HASH, fnv_val);
}

}} // namespace sb::test
