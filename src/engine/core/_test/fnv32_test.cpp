#include <core/hash/fnv.h>
#include <libc++/iterator>

#include <gtest/gtest_common.h>

namespace sb { namespace test {

static ui8 const FNV32_ARRAY_TEST[] = {0x64, 0x7C, 0x31, 0xEC, 0xFD};
static FNV32Value const FNV32_ARRAY_TEST_HASH = 0x644280dfU;

static char const * const FNV32_STR_TEST = "Hello World !";
static FNV32Value const FNV32_STR_TEST_HASH = 0xcad2b6acU;

TEST(FNV32, NullFNV)
{
    EXPECT_EQ(FNV32_OFFSET, computeFNV1a32(nullptr, 0));
    EXPECT_EQ(FNV32_OFFSET, computeFNV1a32((ui8 const * const) "Hello", 0));
    EXPECT_EQ(FNV32_OFFSET, computeFNV1a32(""));
}

TEST(FNV32, ComputeBuffer)
{
    FNV32Value const fnv_val = computeFNV1a32(FNV32_ARRAY_TEST, wstd::size(FNV32_ARRAY_TEST));

    EXPECT_EQ(FNV32_ARRAY_TEST_HASH, fnv_val);
}

TEST(FNV32, ComputeStr)
{
    FNV32Value const fnv_val = computeFNV1a32(FNV32_STR_TEST);

    EXPECT_EQ(FNV32_STR_TEST_HASH, fnv_val);
}

}} // namespace sb::test
