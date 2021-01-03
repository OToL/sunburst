#include <sb_core/container/fix_array.h>
#include <sb_core/bit.h>

#include <sb_unit/test_object_cnt.h>

#include <sb_std/algorithm>
#include <sb_std/iterator>

#include <sb_core/extern_prolog.h>
#include <catch2/catch.hpp>

using namespace sb;

constexpr usize TEST_FARRAY_CAPACITY = 10;
constexpr usize TEST_FARRAY_REF[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

using TestFArrayPOD = FArray<usize, TEST_FARRAY_CAPACITY>;
using TestFArray = FArray<TestObjectCnt, TEST_FARRAY_CAPACITY>;

namespace {

void FillTestFArrayPush(TestFArrayPOD & test_array)
{
    for (usize idx = 0; idx != test_array.capacity(); ++idx)
    {
        test_array.push_back(idx);
    }

    REQUIRE(test_array.size() == test_array.capacity());
}

void FillTestFArrayPush(TestFArray & test_array)
{
    for (usize idx = 0; idx != test_array.capacity(); ++idx)
    {
        test_array.push_back(TestObjectCnt{idx});
    }

    REQUIRE(test_array.size() == test_array.capacity());
    REQUIRE(TestObjectCnt::getStats() == test_array.capacity());
}

void FillTestFArrayEmplace(TestFArray & test_array)
{
    for (usize idx = 0; idx != test_array.capacity(); ++idx)
    {
        test_array.emplace_back(TestObjectCnt{idx});
    }

    REQUIRE(test_array.size() == test_array.capacity());
    REQUIRE(TestObjectCnt::getStats() == test_array.capacity());
}

} // namespace

TEST_CASE("FArray POD default ctor", "[fix_array]")
{
    TestFArrayPOD test_array;

    REQUIRE(test_array.size() == 0ULL);
    REQUIRE(test_array.empty());
}

TEST_CASE("FArray POD push back", "[fix_array]")
{
    TestFArrayPOD test_array;

    FillTestFArrayPush(test_array);

    REQUIRE(sbstd::equal(begin(test_array), end(test_array), sbstd::begin(TEST_FARRAY_REF)));
}

TEST_CASE("FArray POD back", "[fix_array]")
{
    TestFArrayPOD test_array;

    test_array.push_back(0U);
    test_array.push_back(1U);
    test_array.push_back(2U);

    REQUIRE(test_array.back() == 2U);
}

TEST_CASE("FArray push_back dtor", "[fix_array]")
{
    TestObjectCnt::resetStats();

    {
        TestFArray test_array;

        FillTestFArrayPush(test_array);
    }

    REQUIRE(TestObjectCnt::getStats() == 0ULL);
}

TEST_CASE("FArray emplace_back dtor", "[fix_array]")
{
    TestObjectCnt::resetStats();

    {
        TestFArray test_array;

        FillTestFArrayEmplace(test_array);
    }

    REQUIRE(TestObjectCnt::getStats() == 0ULL);
}

TEST_CASE("FArray erase first element", "[fix_array]")
{
    TestObjectCnt::resetStats();

    {
        TestFArray test_array;

        FillTestFArrayPush(test_array);

        test_array.erase(test_array.begin());

        REQUIRE(test_array.size() == (test_array.capacity() - 1));
        REQUIRE(TestObjectCnt::getStats() == 9ULL);

        REQUIRE(sbstd::equal(begin(test_array), end(test_array), sbstd::begin(TEST_FARRAY_REF) + 1));
    }

    REQUIRE(TestObjectCnt::getStats() == 0ULL);
}

TEST_CASE("FArray erase last element", "[fix_array]")
{
    TestObjectCnt::resetStats();

    {
        TestFArray test_array;

        FillTestFArrayPush(test_array);

        test_array.erase(test_array.end() - 1);

        REQUIRE(test_array.size() == (test_array.capacity() - 1));
        REQUIRE(TestObjectCnt::getStats() == (test_array.capacity() - 1));

        REQUIRE(sbstd::equal(begin(test_array), end(test_array) - 1, sbstd::begin(TEST_FARRAY_REF)));
    }

    REQUIRE(TestObjectCnt::getStats() == 0ULL);
}

TEST_CASE("FArray erase middle element", "[fix_array]")
{
    TestObjectCnt::resetStats();

    {
        TestFArray test_array;

        FillTestFArrayPush(test_array);

        constexpr usize ERASE_OFFSET = 2;
        test_array.erase(test_array.begin() + ERASE_OFFSET);

        REQUIRE(test_array.size() == (test_array.capacity() - 1));
        REQUIRE(TestObjectCnt::getStats() == (test_array.capacity() - 1));

        REQUIRE(sbstd::equal(begin(test_array), begin(test_array) + ERASE_OFFSET, sbstd::begin(TEST_FARRAY_REF)));
        REQUIRE(sbstd::equal(begin(test_array) + ERASE_OFFSET, end(test_array),
                             sbstd::begin(TEST_FARRAY_REF) + ERASE_OFFSET + 1));
    }

    REQUIRE(TestObjectCnt::getStats() == 0ULL);
}

TEST_CASE("FArray aligned storage", "[fix_array]")
{
#pragma warning(push)
#pragma warning(disable : 4324)
    struct alignas(32) AlignedType
    {
        u32 value;
    };
#pragma warning(pop)

    FArray<AlignedType, 10> aligned_array;
    aligned_array.emplace_back(1U);

    REQUIRE(isAlignedTo(aligned_array.data(), 32U));
}

#include <sb_core/extern_epilog.h>
