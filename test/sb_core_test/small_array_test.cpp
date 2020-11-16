#include <sb_unit/test_object_cnt.h>
#include <sb_unit/test_allocator.h>

#include <sb_core/container/small_array.h>
#include <sb_core/memory/allocator/allocator_wrapper.h>
#include <sb_core/memory/allocator/container_allocator_wrapper.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/error.h>

#include <sb_std/limits>

#include <catch2/test_prolog.h>
#include <catch2/xcatch.hpp>

using namespace sb;

class SArrayFixture
{
public:
    SArrayFixture()
    {
        TestObjectCnt::resetStats();
    }

    ~SArrayFixture()
    {
        REQUIRE(TestObjectCnt::getStats() == 0U);
    }
};

template <usize CAPACITY>
using SArrayTest = SArray<TestObjectCnt, CAPACITY>;

template <usize CAPACITY>
using SArrayTestA = SArray<TestObjectCnt, CAPACITY, ContainerAllocatorWrapper>;
using SArrayTestAlloc = ContainerAllocatorWrapper;

using SMALL_VECTOR = SArrayFixture;

TEST_CASE_METHOD(SArrayFixture, "Default SArray ctor", "[small_vector]")
{
    SArrayTest<5> vect;

    REQUIRE(vect.empty());
    REQUIRE(5U == vect.capacity());
}

TEST_CASE_METHOD(SArrayFixture, "Set SArray size", "[small_vect]")
{
    SECTION("Set SArray size to the internal storage")
    {
        SArrayTest<5> small_vect(5);

        REQUIRE_FALSE(small_vect.empty());
        REQUIRE(small_vect.size() == 5U);
        REQUIRE(TestObjectCnt::getStats() == 5U);
        REQUIRE(small_vect.isSmallStorage());

        REQUIRE(sbstd::all_of(begin(small_vect), end(small_vect), TestObjectCnt::CompEqual{0}));
    }

    SECTION("Set SArray size to internal storage capacity shall not allocate memory")
    {
        TestAllocator alloc_stats;
        SArrayTestAlloc stl_alloc{alloc_stats};

        SArrayTestA<5> small_vect(5, stl_alloc);

        REQUIRE_FALSE(small_vect.empty());
        REQUIRE(small_vect.size() == 5U);
        REQUIRE(TestObjectCnt::getStats() == 5U);
        REQUIRE(small_vect.isSmallStorage());
        REQUIRE(sbstd::all_of(begin(small_vect), end(small_vect), TestObjectCnt::CompEqual{0}));

        REQUIRE(alloc_stats.getStats().alloc_count == 0ULL);
        REQUIRE_FALSE(alloc_stats.owns(small_vect.data()));
    }

    SECTION("Set SArray size greater than internal storage shall use external memory")
    {
        SArrayTest<5> small_vect(10);

        REQUIRE_FALSE(small_vect.empty());
        REQUIRE(small_vect.size() == 10U);
        REQUIRE(TestObjectCnt::getStats() == 10U);
        REQUIRE(sbstd::all_of(begin(small_vect), end(small_vect), TestObjectCnt::CompEqual{0}));
        REQUIRE_FALSE(small_vect.isSmallStorage());
    }

    SECTION("Set SArray size greater than internal storage shall allocate memory from its allocator")
    {
        TestAllocator alloc_stats;

        {
            SArrayTestAlloc stl_alloc{alloc_stats};
            SArrayTestA<5> small_vect(10, stl_alloc);

            REQUIRE_FALSE(small_vect.empty());
            REQUIRE(small_vect.size() == 10U);
            REQUIRE(TestObjectCnt::getStats() == 10U);
            REQUIRE(sbstd::all_of(begin(small_vect), end(small_vect), TestObjectCnt::CompEqual{0}));
            REQUIRE_FALSE(small_vect.isSmallStorage());

            REQUIRE(alloc_stats.getStats().alloc_count != 0ULL);
            REQUIRE(alloc_stats.owns(small_vect.data()));
        }

        REQUIRE(alloc_stats.getStats().alloc_count == 0ULL);
    }
}

TEST_CASE_METHOD(SArrayFixture, "SArray ctor with default value", "[small_vector]")
{
    TestObjectCnt init_obj{1};

    REQUIRE(TestObjectCnt::getStats() == 1U);

    SECTION("SArray with default value within internal storage")
    {
        {
            SArrayTest<5> small_vect(5, init_obj);

            REQUIRE_FALSE(small_vect.empty());
            REQUIRE(small_vect.size() == 5U);
            REQUIRE(TestObjectCnt::getStats() == 6U);
            REQUIRE(sbstd::all_of(begin(small_vect), end(small_vect), TestObjectCnt::CompEqual{init_obj}));
            REQUIRE(small_vect.isSmallStorage());
        }

        REQUIRE(TestObjectCnt::getStats() == 1U);
    }

    SECTION("SArray with default value within internal storage shall not allocate memory")
    {
        {
            TestAllocator alloc_stats;
            SArrayTestAlloc stl_alloc{alloc_stats};

            SArrayTestA<5> small_vect(5, init_obj, stl_alloc);

            REQUIRE_FALSE(small_vect.empty());
            REQUIRE(small_vect.size() == 5U);
            REQUIRE(TestObjectCnt::getStats() == 6U);
            REQUIRE(sbstd::all_of(begin(small_vect), end(small_vect), TestObjectCnt::CompEqual{init_obj}));
            REQUIRE(small_vect.isSmallStorage());

            REQUIRE(alloc_stats.getStats().alloc_count == 0ULL);
            REQUIRE_FALSE(alloc_stats.owns(small_vect.data()));
        }

        REQUIRE(TestObjectCnt::getStats() == 1U);
    }

    SECTION("SnallVector with default value bigger than internal storage")
    {
        SArrayTest<5> small_vect(10, init_obj);

        REQUIRE_FALSE(small_vect.empty());
        REQUIRE(small_vect.size() == 10U);
        REQUIRE(TestObjectCnt::getStats() == 11U);
        REQUIRE(sbstd::all_of(begin(small_vect), end(small_vect), TestObjectCnt::CompEqual{init_obj}));
        REQUIRE_FALSE(small_vect.isSmallStorage());
    }

    SECTION("SnallVector with default value bigger than internal storage shall allocate external memory")
    {
        TestAllocator alloc_stats;

        {
            SArrayTestAlloc stl_alloc{alloc_stats};

            SArrayTestA<5> small_vect(10, init_obj, stl_alloc);

            REQUIRE_FALSE(small_vect.empty());
            REQUIRE(small_vect.size() == 10U);
            REQUIRE(TestObjectCnt::getStats() == 11U);
            REQUIRE(sbstd::all_of(begin(small_vect), end(small_vect), TestObjectCnt::CompEqual{init_obj}));
            REQUIRE_FALSE(small_vect.isSmallStorage());

            REQUIRE(alloc_stats.getStats().alloc_count == 1ULL);
            REQUIRE(alloc_stats.owns(small_vect.data()));
        }

        REQUIRE(alloc_stats.getStats().alloc_count == 0ULL);
        REQUIRE(TestObjectCnt::getStats() == 1U);
    }
}

void initTestObjectSequence(sbstd::span<TestObjectCnt> objects, usize start_id = 0)
{
    usize obj_id = start_id;

    for (auto & obj : objects)
    {
        obj.setId(obj_id);
        ++obj_id;
    }
}

bool areTestObjectsSequencial(sbstd::span<TestObjectCnt> objects, usize start_id = 0)
{
    usize obj_id = start_id;

    for (auto const & obj : objects)
    {
        if (obj_id != obj.getId())
        {
            return false;
        }

        ++obj_id;
    }

    return true;
}

TEST_CASE_METHOD(SArrayFixture, "SArray ctor range insert", "[small_vector]")
{
    SECTION("Fit in internal storage")
    {
        TestObjectCnt objs[5];
        initTestObjectSequence(objs);

        SECTION("Objects are inserted in order")
        {
            {
                SArrayTest<5> small_vect(sbstd::begin(objs), sbstd::end(objs));

                REQUIRE(!small_vect.empty());
                REQUIRE(small_vect.size() == 5U);
                REQUIRE(TestObjectCnt::getStats() == 10U);
                REQUIRE(sbstd::equal(sbstd::begin(objs), sbstd::end(objs), begin(small_vect)));
            }

            REQUIRE(TestObjectCnt::getStats() == 5U);
        }

        SECTION("No memory is allocated when inserted range is smaller or equal to internal storage")
        {
            TestAllocator alloc_stats;
            SArrayTestAlloc stl_alloc{alloc_stats};

            {
                SArrayTestA<5> small_vect(sbstd::begin(objs), sbstd::end(objs), stl_alloc);

                REQUIRE(!small_vect.empty());
                REQUIRE(small_vect.size() == 5U);
                REQUIRE(small_vect.isSmallStorage());

                REQUIRE(TestObjectCnt::getStats() == 10U);

                REQUIRE(!alloc_stats.owns(small_vect.data()));
                REQUIRE(alloc_stats.getStats().alloc_count == 0ULL);

                REQUIRE(sbstd::equal(sbstd::begin(objs), sbstd::end(objs), begin(small_vect)));
            }

            REQUIRE(TestObjectCnt::getStats() == 5U);
        }
    }

    SECTION("Exceeds internal storage")
    {
        TestObjectCnt objs[10];
        initTestObjectSequence(objs);

        SECTION("Objects are inserted in order")
        {
            {
                SArrayTest<5> small_vect(sbstd::begin(objs), sbstd::end(objs));

                REQUIRE_FALSE(small_vect.empty());
                REQUIRE(small_vect.size() == 10U);
                REQUIRE(TestObjectCnt::getStats() == 20U);
                REQUIRE(sbstd::equal(begin(small_vect), end(small_vect), sbstd::begin(objs)));
            }

            REQUIRE(TestObjectCnt::getStats() == 10U);
        }

        SECTION("Memory is allocated when exceeding internal storage size")
        {
            TestAllocator alloc_stats;
            SArrayTestAlloc stl_alloc{alloc_stats};

            {
                SArrayTestA<5> small_vect(sbstd::begin(objs), sbstd::end(objs), stl_alloc);

                REQUIRE_FALSE(small_vect.empty());
                REQUIRE(small_vect.size() == 10U);
                REQUIRE(sbstd::equal(begin(small_vect), end(small_vect), sbstd::begin(objs)));
                REQUIRE(TestObjectCnt::getStats() == 20U);

                REQUIRE(alloc_stats.getStats().alloc_count == 1U);
                REQUIRE(alloc_stats.owns(small_vect.data()));
            }

            REQUIRE(alloc_stats.getStats().alloc_count == 0U);
            REQUIRE(TestObjectCnt::getStats() == 10U);
        }
    }
}

TEST_CASE_METHOD(SArrayFixture, "SArray copy ctor", "[small_vector]")
{
    SECTION("Objects are copied in order and fit in internal storage")
    {
        SArrayTest<4> src_vec(3);
        initTestObjectSequence(src_vec);
        REQUIRE(src_vec.isSmallStorage());

        REQUIRE(TestObjectCnt::getStats() == 3U);

        {
            SArrayTest<5> dst_vec(src_vec);

            REQUIRE(TestObjectCnt::getStats() == 6U);

            REQUIRE(dst_vec.size() == src_vec.size());
            REQUIRE(sbstd::equal(begin(src_vec), end(src_vec), begin(dst_vec)));
            REQUIRE(dst_vec.isSmallStorage());
        }

        REQUIRE(TestObjectCnt::getStats() == 3U);
    }

    SECTION("Objects are copied in order and no extra memory is allocated when fitting in internal storage")
    {
        TestAllocator src_alloc_stats;
        SArrayTestAlloc src_stl_alloc{src_alloc_stats};

        TestAllocator dst_alloc_stats;
        SArrayTestAlloc dst_stl_alloc{dst_alloc_stats};

        {
            SArrayTestA<4> src_vec(3, src_stl_alloc);
            initTestObjectSequence(src_vec);

            REQUIRE(src_vec.isSmallStorage());
            REQUIRE(src_alloc_stats.getStats().alloc_count == 0U);
            REQUIRE_FALSE(src_alloc_stats.owns(src_vec.data()));
            REQUIRE(TestObjectCnt::getStats() == 3U);

            SArrayTestA<5> dst_vec(src_vec, dst_stl_alloc);

            REQUIRE(TestObjectCnt::getStats() == 6U);
            REQUIRE(dst_vec.size() == src_vec.size());
            REQUIRE(sbstd::equal(begin(src_vec), end(src_vec), begin(dst_vec)));
            REQUIRE(dst_vec.isSmallStorage());
            REQUIRE(dst_alloc_stats.getStats().alloc_count == 0U);
            REQUIRE_FALSE(dst_alloc_stats.owns(dst_vec.data()));
        }

        REQUIRE(dst_alloc_stats.getStats().alloc_count == 0U);
        REQUIRE(src_alloc_stats.getStats().alloc_count == 0U);
    }

    SECTION("Objects are copied in order and internal storage is not used when exceeded")
    {
        SArrayTest<10> src_vec(6);
        initTestObjectSequence(src_vec);
        REQUIRE(src_vec.isSmallStorage());

        REQUIRE(TestObjectCnt::getStats() == 6U);

        SArrayTest<5> dst_vec(src_vec);
        REQUIRE_FALSE(dst_vec.isSmallStorage());

        REQUIRE(TestObjectCnt::getStats() == 12U);

        REQUIRE(dst_vec.size() == src_vec.size());
        REQUIRE(sbstd::equal(begin(src_vec), end(src_vec), begin(dst_vec)));
    }

    SECTION("Objects are copied in order and memory is allocated outside internal storage")
    {
        TestAllocator src_alloc;
        SArrayTestAlloc src_stl_alloc{src_alloc};

        TestAllocator dst_alloc;
        SArrayTestAlloc dst_stl_alloc{dst_alloc};

        {
            SArrayTestA<10> src_vec(6, src_stl_alloc);
            initTestObjectSequence(src_vec);

            REQUIRE(src_vec.isSmallStorage());
            REQUIRE(src_alloc.getStats().alloc_count == 0U);
            REQUIRE_FALSE(src_alloc.owns(src_vec.data()));

            REQUIRE(TestObjectCnt::getStats() == 6U);

            SArrayTestA<5> dst_vec(src_vec, dst_stl_alloc);

            REQUIRE(TestObjectCnt::getStats() == 12U);

            REQUIRE(dst_vec.size() == src_vec.size());
            REQUIRE(sbstd::equal(begin(src_vec), end(src_vec), begin(dst_vec)));
            REQUIRE_FALSE(dst_vec.isSmallStorage());
            REQUIRE(dst_alloc.getStats().alloc_count != 0U);
            REQUIRE(dst_alloc.owns(dst_vec.data()));
        }

        REQUIRE(dst_alloc.getStats().alloc_count == 0U);
        REQUIRE(src_alloc.getStats().alloc_count == 0U);
    }
}

TEST_CASE_METHOD(SArrayFixture, "SArray move ctor", "[small_vector]")
{
    SECTION("Objects moved between internal storage")
    {
        SArrayTest<3> src_vec(3);
        void const * const src_data = src_vec.data();
        initTestObjectSequence(src_vec);

        REQUIRE(src_vec.isSmallStorage());
        REQUIRE(TestObjectCnt::getStats() == 3U);

        SArrayTest<5> dst_vec(sbstd::move(src_vec));

        REQUIRE(dst_vec.size() == 3U);
        REQUIRE(dst_vec.isSmallStorage());
        REQUIRE(src_data != dst_vec.data());

        for (usize idx = 0; idx != dst_vec.size(); ++idx)
        {
            REQUIRE(dst_vec[idx].getId() == idx);
        }

        REQUIRE(TestObjectCnt::getStats() == 3U);
    }

    SECTION("Objects moved between internal storage does not allocate memory")
    {
        TestAllocator alloc;
        SArrayTestAlloc src_alloc{alloc};
        SArrayTestAlloc dst_alloc{alloc};

        SArrayTestA<3> src_vec(3, src_alloc);
        void const * const src_data = src_vec.data();
        initTestObjectSequence(src_vec);

        REQUIRE(src_vec.isSmallStorage());
        REQUIRE_FALSE(alloc.owns(src_vec.data()));
        REQUIRE(alloc.getStats().alloc_count == 0U);

        REQUIRE(TestObjectCnt::getStats() == 3U);

        SArrayTestA<5> dst_vec(sbstd::move(src_vec), dst_alloc);
        REQUIRE(dst_vec.size() == 3U);
        REQUIRE(dst_vec.isSmallStorage());
        REQUIRE(src_data != dst_vec.data());
        REQUIRE_FALSE(alloc.owns(dst_vec.data()));
        REQUIRE(alloc.getStats().alloc_count == 0U);

        for (usize idx = 0; idx != dst_vec.size(); ++idx)
        {
            REQUIRE(dst_vec[idx].getId() == idx);
        }

        REQUIRE(TestObjectCnt::getStats() == 3U);
    }

    SECTION("Objects moved from internal storage to smaller one allocates memory")
    {
        SArrayTest<5> src_vec(5);
        void const * const src_data = src_vec.data();
        initTestObjectSequence(src_vec);
        REQUIRE(src_vec.isSmallStorage());

        REQUIRE(TestObjectCnt::getStats() == 5U);

        SArrayTest<3> dst_vec(sbstd::move(src_vec));
        REQUIRE(dst_vec.size() == 5U);
        REQUIRE_FALSE(dst_vec.isSmallStorage());
        REQUIRE(src_data != dst_vec.data());

        for (usize idx = 0; idx != dst_vec.size(); ++idx)
        {
            REQUIRE(dst_vec[idx].getId() == idx);
        }

        REQUIRE(TestObjectCnt::getStats() == 5U);
    }

    SECTION("Objects moved from bigger to smaller internal storage is allocated memory from allocator")
    {
        TestAllocator test_alloc;

        {
            SArrayTestAlloc src_alloc{test_alloc};
            SArrayTestAlloc dst_alloc{test_alloc};

            SArrayTestA<5> src_vec(5, src_alloc);
            void const * const src_data = src_vec.data();
            initTestObjectSequence(src_vec);

            REQUIRE(src_vec.isSmallStorage());
            REQUIRE_FALSE(test_alloc.owns(src_vec.data()));
            REQUIRE(test_alloc.getStats().alloc_count == 0U);

            REQUIRE(TestObjectCnt::getStats() == 5U);

            SArrayTestA<3> dst_vec(sbstd::move(src_vec), dst_alloc);
            REQUIRE(dst_vec.size() == 5U);

            REQUIRE_FALSE(dst_vec.isSmallStorage());
            REQUIRE(test_alloc.owns(dst_vec.data()));
            REQUIRE(src_data != dst_vec.data());
            REQUIRE(test_alloc.getStats().alloc_count != 0U);

            for (usize idx = 0; idx != dst_vec.size(); ++idx)
            {
                REQUIRE(dst_vec[idx].getId() == idx);
            }

            REQUIRE(TestObjectCnt::getStats() == 5U);
        }

        REQUIRE(test_alloc.getStats().alloc_count == 0U);
    }

    SECTION("Objects are moved from heap to heap")
    {
        SArrayTest<3> src_vec(10);
        auto const src_data = src_vec.data();
        initTestObjectSequence(src_vec);
        REQUIRE_FALSE(src_vec.isSmallStorage());

        REQUIRE(TestObjectCnt::getStats() == 10U);

        SArrayTest<5> dst_vec(sbstd::move(src_vec));
        REQUIRE(dst_vec.size() == 10U);

        REQUIRE_FALSE(dst_vec.isSmallStorage());
        REQUIRE(src_data == dst_vec.data());

        for (usize idx = 0; idx != dst_vec.size(); ++idx)
        {
            REQUIRE(dst_vec[idx].getId() == idx);
        }

        REQUIRE(TestObjectCnt::getStats() == 10U);
    }

    SECTION("Objects moved from heap to heap is not causing extra allocations")
    {
        TestAllocator test_alloc;

        {
            SArrayTestAlloc src_alloc{test_alloc};
            SArrayTestAlloc dst_alloc{test_alloc};

            SArrayTestA<3> src_vec(10, src_alloc);
            auto const src_data = src_vec.data();
            initTestObjectSequence(src_vec);

            REQUIRE_FALSE(src_vec.isSmallStorage());
            REQUIRE(test_alloc.owns(src_vec.data()));
            REQUIRE(test_alloc.getStats().alloc_count != 0U);

            REQUIRE(TestObjectCnt::getStats() == 10U);

            SArrayTestA<5> dst_vec(sbstd::move(src_vec), dst_alloc);
            REQUIRE(dst_vec.size() == 10U);
            REQUIRE_FALSE(dst_vec.isSmallStorage());
            REQUIRE(src_data == dst_vec.data());
            REQUIRE(test_alloc.owns(dst_vec.data()));
            REQUIRE(test_alloc.getStats().alloc_count != 0U);

            for (usize idx = 0; idx != dst_vec.size(); ++idx)
            {
                REQUIRE(dst_vec[idx].getId() == idx);
            }

            REQUIRE(TestObjectCnt::getStats() == 10U);
        }

        REQUIRE(test_alloc.getStats().alloc_count == 0U);
    }

    SECTION("Move from heap to small storage")
    {
        SArrayTest<5> src_vec(10);
        void const * const src_data = src_vec.data();

        initTestObjectSequence(src_vec);
        REQUIRE_FALSE(src_vec.isSmallStorage());

        REQUIRE(TestObjectCnt::getStats() == 10U);

        SArrayTest<15> dst_vec(sbstd::move(src_vec));

        for (usize idx = 0; idx != dst_vec.size(); ++idx)
        {
            REQUIRE(dst_vec[idx].getId() == idx);
        }

        REQUIRE(dst_vec.size() == 10U);
        REQUIRE(dst_vec.isSmallStorage());
        REQUIRE(src_data != dst_vec.data());

        REQUIRE(TestObjectCnt::getStats() == 10U);
    }

    SECTION("Move from heap to small storage does not free source memory")
    {
        TestAllocator test_alloc;

        {
            SArrayTestAlloc src_alloc{test_alloc};
            SArrayTestAlloc dst_alloc{test_alloc};

            SArrayTestA<5> src_vec(10, src_alloc);
            void const * const src_data = src_vec.data();
            initTestObjectSequence(src_vec);

            REQUIRE_FALSE(src_vec.isSmallStorage());
            REQUIRE(test_alloc.owns(src_vec.data()));

            REQUIRE(TestObjectCnt::getStats() == 10U);

            SArrayTestA<15> dst_vec(sbstd::move(src_vec), dst_alloc);

            for (usize idx = 0; idx != dst_vec.size(); ++idx)
            {
                REQUIRE(dst_vec[idx].getId() == idx);
            }

            REQUIRE(dst_vec.size() == 10U);
            REQUIRE(dst_vec.isSmallStorage());
            REQUIRE(src_data != dst_vec.data());
            REQUIRE_FALSE(test_alloc.owns(dst_vec.data()));

            REQUIRE(TestObjectCnt::getStats() == 10U);
        }

        REQUIRE(test_alloc.getStats().alloc_count == 0U);
    }
}

TEST_CASE_METHOD(SArrayFixture, "SArray emplace back", "[small_vector]")
{
    SArrayTest<5> vect;

    auto const & new_value = vect.emplace_back(0xFFU);
    REQUIRE_FALSE(vect.empty());
    REQUIRE(1U == vect.size());
    REQUIRE(new_value.getId() == 0xFFU);
    REQUIRE(TestObjectCnt::getStats() == 1U);
}

TEST_CASE_METHOD(SArrayFixture, "SArray push back", "[small_vector]")
{
    SECTION("Push lvalue")
    {
        SArrayTest<5> vect;

        TestObjectCnt const obj{0xFFU};
        REQUIRE(TestObjectCnt::getStats() == 1U);

        vect.push_back(obj);

        REQUIRE_FALSE(vect.empty());
        REQUIRE(1U == vect.size());
        REQUIRE(vect.back().getId() == 0xFFU);

        REQUIRE(TestObjectCnt::getStats() == 2U);
    }

    SECTION("Push rvalue")
    {
        SArrayTest<5> vect;

        vect.push_back({0xFFU});

        REQUIRE_FALSE(vect.empty());
        REQUIRE(1U == vect.size());
        REQUIRE(vect.back().getId() == 0xFFU);

        REQUIRE(TestObjectCnt::getStats() == 1U);
    }
}

TEST_CASE_METHOD(SArrayFixture, "SArray clear", "[small_vector]")
{
    SECTION("Fit in internal storage")
    {
        SArrayTest<5> vect(3);

        REQUIRE(TestObjectCnt::getStats() == 3U);

        vect.clear();

        REQUIRE(vect.empty());
        REQUIRE(vect.size() == 0U);

        REQUIRE(TestObjectCnt::getStats() == 0U);
    }

    SECTION("Clearing when not using internal does not free memory")
    {
        TestAllocator test_alloc;

        {
            SArrayTestAlloc src_alloc{test_alloc};
            SArrayTestA<5> vect(10U, src_alloc);
            initTestObjectSequence(vect);

            REQUIRE_FALSE(vect.empty());
            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(10U == vect.size());
            REQUIRE(test_alloc.getStats().alloc_count != 0U);
            REQUIRE(TestObjectCnt::getStats() == 10U);

            vect.clear();

            REQUIRE(vect.empty());
            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(0U == vect.size());
            REQUIRE(test_alloc.getStats().alloc_count != 0U);
            REQUIRE(TestObjectCnt::getStats() == 0U);
        }

        REQUIRE(test_alloc.getStats().alloc_count == 0U);
    }
}

TEST_CASE_METHOD(SArrayFixture, "SArray sub-script accessors", "[small_vector]")
{
    SArrayTest<5> vect{5};
    initTestObjectSequence(vect);

    for (usize idx = 0; idx != 5; ++idx)
    {
        REQUIRE(vect.at(idx).getId() == idx);
        REQUIRE(vect[idx].getId() == idx);
    }

    REQUIRE(vect.front().getId() == 0U);
    REQUIRE(vect.back().getId() == 4U);
}

TEST_CASE_METHOD(SArrayFixture, "SArray capacity reservation", "[small_vector]")
{
    SECTION("Reserving within internal storage does not allocate memory")
    {
        SArrayTest<5> vect(2);
        auto const small_storage_data = vect.data();

        REQUIRE(vect.isSmallStorage());
        REQUIRE(vect.size() == 2U);
        REQUIRE(TestObjectCnt::getStats() == 2U);

        vect.reserve(5);

        REQUIRE(vect.isSmallStorage());
        REQUIRE(vect.size() == 2U);
        REQUIRE(vect.capacity() == 5U);
        REQUIRE(small_storage_data == vect.data());
        REQUIRE(TestObjectCnt::getStats() == 2U);
    }

    SECTION("Reserving less than the current capacity leaves the vector capacity and size unchanged")
    {
        SArrayTest<5> vect(2);
        auto const small_storage_data = vect.data();

        REQUIRE(vect.isSmallStorage());
        REQUIRE(vect.size() == 2U);
        REQUIRE(TestObjectCnt::getStats() == 2U);

        vect.reserve(1);

        REQUIRE(vect.isSmallStorage());
        REQUIRE(vect.size() == 2U);
        REQUIRE(vect.capacity() == 5U);
        REQUIRE(small_storage_data == vect.data());
        REQUIRE(TestObjectCnt::getStats() == 2U);
    }

    SECTION("Reserving more than the internal storage allocate from the heap")
    {
        TestAllocator test_alloc;

        {
            SArrayTestAlloc vect_alloc{test_alloc};
            SArrayTestA<5> vect(3U, vect_alloc);
            initTestObjectSequence(vect);

            auto const small_storage_data = vect.data();

            REQUIRE(vect.isSmallStorage());
            REQUIRE(vect.size() == 3U);
            REQUIRE(TestObjectCnt::getStats() == 3U);
            REQUIRE(test_alloc.getStats().alloc_count == 0U);

            vect.reserve(10U);

            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(vect.size() == 3U);
            REQUIRE(TestObjectCnt::getStats() == 3U);
            REQUIRE(test_alloc.getStats().alloc_count != 0U);
            REQUIRE(small_storage_data != vect.data());

            for (usize idx = 0; idx != vect.size(); ++idx)
            {
                REQUIRE(vect[idx].getId() == idx);
            }
        }

        REQUIRE(test_alloc.getStats().alloc_count == 0U);
    }
}

TEST_CASE_METHOD(SArrayFixture, "SArray shrink to fit", "[small_vector]")
{
    SECTION("Shrink to fit has no effect when fitting in small storage")
    {
        SArrayTest<5> vect(3);
        initTestObjectSequence(vect);

        REQUIRE(vect.isSmallStorage());
        REQUIRE(vect.capacity() == 5U);
        REQUIRE(vect.size() == 3U);
        REQUIRE(TestObjectCnt::getStats() == 3U);

        vect.shrink_to_fit();

        REQUIRE(vect.isSmallStorage());
        REQUIRE(vect.capacity() == 5U);
        REQUIRE(vect.size() == 3U);
        REQUIRE(TestObjectCnt::getStats() == 3U);
    }

    SECTION("Shrink to fit adjust allocated memory outside small storage")
    {
        TestAllocator test_alloc;

        {
            SArrayTestAlloc vect_alloc{test_alloc};
            SArrayTestA<5> vect(vect_alloc);
            auto const small_vect_data = vect.data();

            vect.resize(7U);
            initTestObjectSequence(vect);

            vect.reserve(10U);
            auto const prev_alloc_mem = test_alloc.getStats().allocated_byte;

            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(TestObjectCnt::getStats() == 7U);
            REQUIRE(test_alloc.getStats().alloc_count != 0U);
            REQUIRE(small_vect_data != vect.data());

            vect.shrink_to_fit();

            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(TestObjectCnt::getStats() == 7U);
            REQUIRE(test_alloc.getStats().alloc_count != 0U);
            REQUIRE(test_alloc.getStats().allocated_byte < prev_alloc_mem);
            REQUIRE(small_vect_data != vect.data());

            for (usize idx = 0; idx != vect.size(); ++idx)
            {
                REQUIRE(vect[idx].getId() == idx);
            }
        }

        REQUIRE(test_alloc.getStats().alloc_count == 0U);
        REQUIRE(test_alloc.getStats().allocated_byte == 0U);
    }

    SECTION("Shrink to fit moves the objects back to small storage")
    {
        TestAllocator test_alloc;

        {
            SArrayTestAlloc vect_alloc{test_alloc};
            SArrayTestA<5> vect{3, vect_alloc};
            auto const small_vect_data = vect.data();
            initTestObjectSequence(vect);

            vect.reserve(10U);

            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(TestObjectCnt::getStats() == 3U);
            REQUIRE(test_alloc.getStats().alloc_count != 0U);
            REQUIRE(small_vect_data != vect.data());

            vect.shrink_to_fit();

            REQUIRE(vect.isSmallStorage());
            REQUIRE(TestObjectCnt::getStats() == 3U);
            REQUIRE(test_alloc.getStats().alloc_count == 0U);
            REQUIRE(small_vect_data == vect.data());

            for (usize idx = 0; idx != vect.size(); ++idx)
            {
                REQUIRE(vect[idx].getId() == idx);
            }
        }
    }
}

TEST_CASE_METHOD(SArrayFixture, "SArray pop back", "[small_vector]")
{
    SArrayTest<5> vect(3);
    initTestObjectSequence(vect);

    REQUIRE(vect.size() == 3U);
    REQUIRE(vect.capacity() == 5U);
    REQUIRE(TestObjectCnt::getStats() == 3U);

    vect.pop_back();

    REQUIRE(vect.size() == 2U);
    REQUIRE(vect.capacity() == 5U);
    REQUIRE(TestObjectCnt::getStats() == 2U);

    for (usize idx = 0; idx != vect.size(); ++idx)
    {
        REQUIRE(idx == vect[idx].getId());
    }
}

TEST_CASE_METHOD(SArrayFixture, "SArray maximum size", "[small_vector]")
{
    SArrayTest<5> vect;
    REQUIRE(sbstd::numeric_limits<SArrayTest<5>::size_type>::max() == vect.max_size());
}

TEST_CASE_METHOD(SArrayFixture, "SArray resize", "[small_vector]")
{
    SECTION("Resize using the same size does not affect the vector")
    {
        SArrayTest<5> vect(2U);
        initTestObjectSequence(vect);

        REQUIRE(vect.size() == 2U);
        REQUIRE(vect.isSmallStorage());
        REQUIRE(TestObjectCnt::getStats() == 2U);

        vect.resize(2U);

        REQUIRE(vect.size() == 2U);
        REQUIRE(vect.isSmallStorage());
        REQUIRE(TestObjectCnt::getStats() == 2U);
    }

    SECTION("Resize within internal storage does not allocate memory")
    {
        SArrayTest<5> vect(2U);
        initTestObjectSequence(vect);

        REQUIRE(vect.size() == 2U);
        REQUIRE(vect.isSmallStorage());
        REQUIRE(TestObjectCnt::getStats() == 2U);

        vect.resize(4U);

        REQUIRE(vect.size() == 4U);
        REQUIRE(vect.isSmallStorage());
        REQUIRE(TestObjectCnt::getStats() == 4U);

        for (usize idx = 0; idx != 2U; ++idx)
        {
            REQUIRE(idx == vect[idx].getId());
        }

        REQUIRE(sbstd::all_of(begin(vect) + 2, end(vect), TestObjectCnt::CompEqual{0U}));
    }

    SECTION("Resize with greater value than internal storage allocates memory")
    {
        TestAllocator test_alloc;

        {
            SArrayTestAlloc vect_alloc{test_alloc};
            SArrayTestA<5> vect{2, vect_alloc};
            auto const small_storage_data = vect.data();
            initTestObjectSequence(vect);

            REQUIRE(vect.size() == 2U);
            REQUIRE(vect.isSmallStorage());
            REQUIRE(TestObjectCnt::getStats() == 2U);
            REQUIRE(test_alloc.getStats().alloc_count == 0U);

            vect.resize(10);

            REQUIRE(vect.size() == 10U);
            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(TestObjectCnt::getStats() == 10U);
            REQUIRE(test_alloc.getStats().alloc_count != 0U);
            REQUIRE(vect.data() != small_storage_data);

            for (usize idx = 0; idx != 2U; ++idx)
            {
                REQUIRE(idx == vect[idx].getId());
            }

            REQUIRE(sbstd::all_of(begin(vect) + 2, end(vect), TestObjectCnt::CompEqual{0U}));
        }

        REQUIRE(test_alloc.getStats().alloc_count == 0U);
    }

    SECTION("Resize to a smaller size destroys objects and does not deallocate memory")
    {
        TestAllocator test_alloc;

        {
            SArrayTestAlloc vect_alloc{test_alloc};
            SArrayTestA<5> vect{7, vect_alloc};
            auto const vect_data = vect.data();
            initTestObjectSequence(vect);

            REQUIRE(vect.size() == 7U);
            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(TestObjectCnt::getStats() == 7U);
            REQUIRE(test_alloc.getStats().alloc_count != 0U);

            vect.resize(2);

            REQUIRE(vect.size() == 2U);
            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(TestObjectCnt::getStats() == 2U);
            REQUIRE(test_alloc.getStats().alloc_count != 0U);
            REQUIRE(vect.data() == vect_data);

            for (usize idx = 0; idx != 2U; ++idx)
            {
                REQUIRE(idx == vect[idx].getId());
            }
        }

        REQUIRE(test_alloc.getStats().alloc_count == 0U);
    }

    SECTION("Resize with value")
    {
        SArrayTest<5> vect(2U);
        initTestObjectSequence(vect);

        REQUIRE(vect.size() == 2U);
        REQUIRE(vect.isSmallStorage());
        REQUIRE(TestObjectCnt::getStats() == 2U);

        vect.resize(4U, 2U);

        REQUIRE(vect.size() == 4U);
        REQUIRE(vect.isSmallStorage());
        REQUIRE(TestObjectCnt::getStats() == 4U);

        for (usize idx = 0; idx != 2U; ++idx)
        {
            REQUIRE(idx == vect[idx].getId());
        }

        REQUIRE(sbstd::all_of(begin(vect) + 2, end(vect), TestObjectCnt::CompEqual{2U}));
    }
}

TEST_CASE_METHOD(SArrayFixture, "SArray assign value", "[small_vector]")
{
    SECTION("Assign multiple times an object in small storage")
    {
        TestObjectCnt const REF_OBJECT(0xFFU);

        SArrayTest<5> vect(3);
        initTestObjectSequence(vect);

        REQUIRE(vect.isSmallStorage());
        REQUIRE(vect.size() == 3U);
        REQUIRE(TestObjectCnt::getStats() == 4U);

        vect.assign(4, REF_OBJECT);

        REQUIRE(vect.size() == 4U);
        REQUIRE(vect.isSmallStorage());
        REQUIRE(sbstd::all_of(begin(vect), end(vect), TestObjectCnt::CompEqual{REF_OBJECT}));
        REQUIRE(TestObjectCnt::getStats() == 5U);
    }

    SECTION("Assign less objects detroys")
    {
        TestObjectCnt const REF_OBJECT(0xFFU);

        SArrayTest<5> vect(3);
        initTestObjectSequence(vect);

        REQUIRE(vect.isSmallStorage());
        REQUIRE(vect.size() == 3U);
        REQUIRE(TestObjectCnt::getStats() == 4U);

        vect.assign(2, REF_OBJECT);

        REQUIRE(vect.size() == 2U);
        REQUIRE(vect.isSmallStorage());
        REQUIRE(sbstd::all_of(begin(vect), end(vect), TestObjectCnt::CompEqual{REF_OBJECT}));
        REQUIRE(TestObjectCnt::getStats() == 3U);
    }

    SECTION("Assign more objects allocates memory")
    {
        TestObjectCnt const REF_OBJECT(0xFFU);
        TestAllocator test_alloc;

        {
            SArrayTestAlloc vect_alloc{test_alloc};
            SArrayTestA<5> vect(3, vect_alloc);
            auto const small_vect_data = vect.data();
            initTestObjectSequence(vect);

            REQUIRE(vect.isSmallStorage());
            REQUIRE(vect.size() == 3U);
            REQUIRE(TestObjectCnt::getStats() == 4U);
            REQUIRE(test_alloc.getStats().alloc_count == 0U);

            vect.assign(10, REF_OBJECT);

            REQUIRE(vect.size() == 10U);
            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(sbstd::all_of(begin(vect), end(vect), TestObjectCnt::CompEqual{REF_OBJECT}));
            REQUIRE(TestObjectCnt::getStats() == 11U);
            REQUIRE(test_alloc.getStats().alloc_count != 0U);
            REQUIRE(vect.data() != small_vect_data);
        }

        REQUIRE(test_alloc.getStats().alloc_count == 0U);
    }

    SECTION("Assign objects which fits in internal storage does not deallocate memory")
    {
        TestObjectCnt const REF_OBJECT(0xFFU);
        TestAllocator test_alloc;

        {
            SArrayTestAlloc vect_alloc{test_alloc};
            SArrayTestA<5> vect(10, vect_alloc);
            auto const vect_data = vect.data();
            initTestObjectSequence(vect);

            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(vect.size() == 10U);
            REQUIRE(TestObjectCnt::getStats() == 11U);
            REQUIRE(test_alloc.getStats().alloc_count != 0U);

            vect.assign(2, REF_OBJECT);

            REQUIRE(vect.size() == 2U);
            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(vect.data() == vect_data);
            REQUIRE(sbstd::all_of(begin(vect), end(vect), TestObjectCnt::CompEqual{REF_OBJECT}));
            REQUIRE(TestObjectCnt::getStats() == 3U);
            REQUIRE(test_alloc.getStats().alloc_count != 0U);
        }

        REQUIRE(test_alloc.getStats().alloc_count == 0U);
    }
}

TEST_CASE_METHOD(SArrayFixture, "SArray assign operator", "[small_vector]")
{
    SECTION("Assign a smaller vector which fits in internal storage while using interal storage")
    {
        SArrayTest<3> vect_src(3);
        initTestObjectSequence(vect_src);

        REQUIRE(TestObjectCnt::getStats() == 3U);
        REQUIRE(vect_src.isSmallStorage());

        SArrayTest<5> vect(5);
        initTestObjectSequence(vect, 10);

        REQUIRE(TestObjectCnt::getStats() == 8U);
        REQUIRE(vect.isSmallStorage());

        vect = vect_src;

        REQUIRE(vect.size() == 3U);
        REQUIRE(TestObjectCnt::getStats() == 6U);
        REQUIRE(vect.isSmallStorage());

        for (usize idx = 0; idx != 3U; ++idx)
        {
            REQUIRE(idx == vect[idx].getId());
        }
    }

    SECTION("Assign a vector larger than internal storage allocates memory")
    {
        TestAllocator test_alloc;

        {
            SArrayTest<5> vect_src(5);
            initTestObjectSequence(vect_src);

            REQUIRE(TestObjectCnt::getStats() == 5U);
            REQUIRE(vect_src.isSmallStorage());

            SArrayTestAlloc vect_alloc{test_alloc};
            SArrayTestA<3> vect(3, vect_alloc);
            initTestObjectSequence(vect, 10);

            REQUIRE(TestObjectCnt::getStats() == 8U);
            REQUIRE(vect.isSmallStorage());
            REQUIRE(test_alloc.getStats().alloc_count == 0U);

            vect = vect_src;

            REQUIRE(vect.size() == 5U);
            REQUIRE(TestObjectCnt::getStats() == 10U);
            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(test_alloc.getStats().alloc_count != 0U);

            for (usize idx = 0; idx != 5U; ++idx)
            {
                REQUIRE(idx == vect[idx].getId());
            }
        }

        REQUIRE(test_alloc.getStats().alloc_count == 0U);
    }

    SECTION("Assign a vector fitting in small storage does not deallocate memory")
    {
        TestAllocator test_alloc;

        {
            SArrayTest<3> vect_src(3);
            initTestObjectSequence(vect_src);

            REQUIRE(TestObjectCnt::getStats() == 3U);
            REQUIRE(vect_src.isSmallStorage());

            SArrayTestAlloc vect_alloc{test_alloc};
            SArrayTestA<3> vect(10, vect_alloc);
            initTestObjectSequence(vect, 10);

            REQUIRE(TestObjectCnt::getStats() == 13U);
            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(test_alloc.getStats().alloc_count != 0U);

            vect = vect_src;

            REQUIRE(vect.size() == 3U);
            REQUIRE(TestObjectCnt::getStats() == 6U);
            REQUIRE_FALSE(vect.isSmallStorage());
            REQUIRE(test_alloc.getStats().alloc_count != 0U);

            for (usize idx = 0; idx != 3U; ++idx)
            {
                REQUIRE(idx == vect[idx].getId());
            }
        }

        REQUIRE(test_alloc.getStats().alloc_count == 0U);
    }
}

TEST_CASE_METHOD(SArrayFixture, "SArray swap", "[small_vector]")
{
    SECTION("Swap heap storage")
    {
        SArrayTest<3> vect1(10), vect2(5);

        initTestObjectSequence(vect1);
        initTestObjectSequence(vect2, 20U);

        REQUIRE(TestObjectCnt::getStats() == 15U);
        REQUIRE(vect1.size() == 10U);
        REQUIRE(vect2.size() == 5U);
        REQUIRE_FALSE(vect1.isSmallStorage());
        REQUIRE_FALSE(vect2.isSmallStorage());
        REQUIRE(areTestObjectsSequencial(vect1));
        REQUIRE(areTestObjectsSequencial(vect2, 20U));

        auto const * const vect1_data = vect1.data();
        auto const * const vect2_data = vect2.data();

        vect1.swap(vect2);

        REQUIRE(TestObjectCnt::getStats() == 15U);
        REQUIRE(vect2.size() == 10U);
        REQUIRE(vect1.size() == 5U);
        REQUIRE_FALSE(vect1.isSmallStorage());
        REQUIRE_FALSE(vect2.isSmallStorage());
        REQUIRE(areTestObjectsSequencial(vect2));
        REQUIRE(areTestObjectsSequencial(vect1, 20U));

        REQUIRE(vect1_data == vect2.data());
        REQUIRE(vect2_data == vect1.data());
    }

    SECTION("Swap vectors using internal storage")
    {
        SArrayTest<20> vect1(10), vect2(5);

        initTestObjectSequence(vect1);
        initTestObjectSequence(vect2, 20U);

        REQUIRE(TestObjectCnt::getStats() == 15U);
        REQUIRE(vect1.size() == 10U);
        REQUIRE(vect2.size() == 5U);
        REQUIRE(vect1.isSmallStorage());
        REQUIRE(vect2.isSmallStorage());
        REQUIRE(areTestObjectsSequencial(vect1));
        REQUIRE(areTestObjectsSequencial(vect2, 20U));

        vect1.swap(vect2);

        REQUIRE(TestObjectCnt::getStats() == 15U);
        REQUIRE(vect2.size() == 10U);
        REQUIRE(vect1.size() == 5U);
        REQUIRE(vect2.isSmallStorage());
        REQUIRE(vect1.isSmallStorage());
        REQUIRE(areTestObjectsSequencial(vect2));
        REQUIRE(areTestObjectsSequencial(vect1, 20U));
    }

    SECTION("Swap from heap to internal storage")
    {
        SArrayTest<5> vect1(5), vect2(10);

        initTestObjectSequence(vect1);
        initTestObjectSequence(vect2, 20U);

        REQUIRE(TestObjectCnt::getStats() == 15U);
        REQUIRE(vect1.size() == 5U);
        REQUIRE(vect2.size() == 10U);
        REQUIRE(vect1.isSmallStorage());
        REQUIRE_FALSE(vect2.isSmallStorage());
        REQUIRE(areTestObjectsSequencial(vect1));
        REQUIRE(areTestObjectsSequencial(vect2, 20U));

        auto const * const vect1_data = vect1.data();
        auto const * const vect2_data = vect2.data();

        vect1.swap(vect2);

        REQUIRE(TestObjectCnt::getStats() == 15U);
        REQUIRE(vect2.size() == 5U);
        REQUIRE(vect1.size() == 10U);
        REQUIRE_FALSE(vect1.isSmallStorage());
        REQUIRE_FALSE(vect2.isSmallStorage());
        REQUIRE(areTestObjectsSequencial(vect2));
        REQUIRE(areTestObjectsSequencial(vect1, 20U));
        REQUIRE(vect2.data() == vect2_data);
        REQUIRE(vect1.data() != vect1_data);
    }

    SECTION("Swap from internal to heap storage")
    {
        SArrayTest<5> vect2(5), vect1(10);

        initTestObjectSequence(vect2);
        initTestObjectSequence(vect1, 20U);

        REQUIRE(TestObjectCnt::getStats() == 15U);
        REQUIRE(vect2.size() == 5U);
        REQUIRE(vect1.size() == 10U);
        REQUIRE(vect2.isSmallStorage());
        REQUIRE_FALSE(vect1.isSmallStorage());
        REQUIRE(areTestObjectsSequencial(vect2));
        REQUIRE(areTestObjectsSequencial(vect1, 20U));

        auto const * const vect2_data = vect2.data();
        auto const * const vect1_data = vect1.data();

        vect1.swap(vect2);

        REQUIRE(TestObjectCnt::getStats() == 15U);
        REQUIRE(vect1.size() == 5U);
        REQUIRE(vect2.size() == 10U);
        REQUIRE_FALSE(vect2.isSmallStorage());
        REQUIRE_FALSE(vect1.isSmallStorage());
        REQUIRE(areTestObjectsSequencial(vect1));
        REQUIRE(areTestObjectsSequencial(vect2, 20U));
        REQUIRE(vect1.data() == vect1_data);
        REQUIRE(vect2.data() != vect2_data);
    }
}

TEST_CASE_METHOD(SArrayFixture, "SArray without allocator has not overhead", "[small_vector]")
{
    // sizeof(m_begin) + sizeof(m_end) + sizeof(m_storage_end) + sizeof(small storage test = void *)
    // STATIC_REQUIRE(sizeof(SArray<b8, sizeof(void *)>) == 4 * sizeof(void *));
}

#include <catch2/test_epilog.h>
