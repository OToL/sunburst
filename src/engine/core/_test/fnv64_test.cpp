#include <core/hash/fnv.h>
#include <libc++/iterator>

#include <gtest/gtest_common.h>

namespace sb { namespace test {

static ui8 const FNV64_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static FNV64Value const FNV64_ARRAY_TEST_HASH = 0x23ae0f3c3e4e7adfULL;

static char const * const FNV64_STR_TEST = "Hello World !";
static FNV64Value const FNV64_STR_TEST_HASH = 0x9b90b3ce8e37ea0cULL;

TEST(FNV64, NullFNV)
{
    EXPECT_EQ(FNV64_OFFSET, computeFNV1a64(nullptr, 0));
    EXPECT_EQ(FNV64_OFFSET, computeFNV1a64((ui8 const * const) "Hello", 0));
    EXPECT_EQ(FNV64_OFFSET, computeFNV1a64(""));
}

TEST(FNV64, ComputeBuffer)
{
    FNV64Value const fnv_val = computeFNV1a64(FNV64_ARRAY_TEST, wstd::size(FNV64_ARRAY_TEST));

    EXPECT_EQ(FNV64_ARRAY_TEST_HASH, fnv_val);
}

TEST(FNV64, ComputeStr)
{
    FNV64Value const fnv_val = computeFNV1a64(FNV64_STR_TEST);

    EXPECT_EQ(FNV64_STR_TEST_HASH, fnv_val);
}

}} // namespace sb::test
