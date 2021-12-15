#include <sb_unit/test_object_cnt.h>
#include <sb_unit/test_allocator.h>

#include <sb_core/container/dynamic_small_array.h>
#include <sb_core/memory/allocator/allocator_wrapper.h>
#include <sb_core/memory/allocator/container_allocator_wrapper.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/error.h>

#include <sb_std/limits>

#include <extern_prolog.h>
#include <catch2/catch.hpp>

using namespace sb;

class DSArrayFixture
{
public:
    DSArrayFixture()
    {
        TestObjectCnt::resetStats();
    }

    DSArrayFixture(DSArrayFixture const &) = default;
    DSArrayFixture(DSArrayFixture &&) = default;
    DSArrayFixture & operator=(DSArrayFixture const &) = default;
    DSArrayFixture & operator=(DSArrayFixture &&) = default;

    ~DSArrayFixture()
    {
        REQUIRE(TestObjectCnt::getStats() == 0U);
    }
};

template <usize CAPACITY>
using DSArrayTest = DSArray<TestObjectCnt, CAPACITY>;

template <usize CAPACITY>
using DSArrayTestA = DSArray<TestObjectCnt, CAPACITY, ContainerAllocatorWrapper>;
using DSArrayTestAlloc = ContainerAllocatorWrapper;

using SMALL_VECTOR = DSArrayFixture;

TEST_CASE_METHOD(DSArrayFixture, "Default DSArray ctor", "[dyamic_small_array]")
{
    DSArrayTest<5> vect;

    REQUIRE(vect.empty());
    REQUIRE(5U == vect.capacity());
}

TEST_CASE_METHOD(DSArrayFixture, "Set DSArray size", "[small_vect]")
{
    SECTION("Set DSArray size to the internal storage")
    {
        DSArrayTest<5> small_vect(5);

        REQUIRE_FALSE(small_vect.empty());
        REQUIRE(small_vect.size() == 5U);
        REQUIRE(TestObjectCnt::getStats() == 5U);
        REQUIRE(small_vect.isSmallStorage());

        REQUIRE(sbstd::all_of(begin(small_vect), end(small_vect), TestObjectCnt::CompEqual{0}));
    }

    SECTION("Set DSArray size to internal storage capacity shall not allocate memory")
    {
        TestAllocator alloc_stats;
        DSArrayTestAlloc stl_alloc{alloc_stats};

        DSArrayTestA<5> small_vect(5, stl_alloc);

        REQUIRE_FALSE(small_vect.empty());
        REQUIRE(small_vect.size() == 5U);
        REQUIRE(TestObjectCnt::getStats() == 5U);
        REQUIRE(small_vect.isSmallStorage());
        REQUIRE(sbstd::all_of(begin(small_vect), end(small_vect), TestObjectCnt::CompEqual{0}));

        REQUIRE(alloc_stats.getStats().alloc_count == 0ULL);
        REQUIRE_FALSE(alloc_stats.owns(small_vect.data()));
    }

    SECTION("Set DSArray size greater than internal storage shall use external memory")
    {
        DSArrayTest<5> small_vect(10);

        REQUIRE_FALSE(small_vect.empty());
        REQUIRE(small_vect.size() == 10U);
        REQUIRE(TestObjectCnt::getStats() == 10U);
        REQUIRE(sbstd::all_of(begin(small_vect), end(small_vect), TestObjectCnt::CompEqual{0}));
        REQUIRE_FALSE(small_vect.isSmallStorage());
    }

    SECTION("Set DSArray size greater than internal storage shall allocate memory from its allocator")
    {
        TestAllocator alloc_stats;

        {
            DSArrayTestAlloc stl_alloc{alloc_stats};
            DSArrayTestA<5> small_vect(10, stl_alloc);

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

TEST_CASE_METHOD(DSArrayFixture, "DSArray ctor with default value", "[dyamic_small_array]")
{
    TestObjectCnt init_obj{1};

    REQUIRE(TestObjectCnt::getStats() == 1U);

    SECTION("DSArray with default value within internal storage")
    {
        {
            DSArrayTest<5> small_vect(5, init_obj);

            REQUIRE_FALSE(small_vect.empty());
            REQUIRE(small_vect.size() == 5U);
            REQUIRE(TestObjectCnt::getStats() == 6U);
            REQUIRE(sbstd::all_of(begin(small_vect), end(small_vect), TestObjectCnt::CompEqual{init_obj}));
            REQUIRE(small_vect.isSmallStorage());
        }

        REQUIRE(TestObjectCnt::getStats() == 1U);
    }

    SECTION("DSArray with default value within internal storage shall not allocate memory")
    {
        {
            TestAllocator alloc_stats;
            DSArrayTestAlloc stl_alloc{alloc_stats};

            DSArrayTestA<5> small_vect(5, init_obj, stl_alloc);

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
        DSArrayTest<5> small_vect(10, init_obj);

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
            DSArrayTestAlloc stl_alloc{alloc_stats};

            DSArrayTestA<5> small_vect(10, init_obj, stl_alloc);

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

TEST_CASE_METHOD(DSArrayFixture, "DSArray ctor range insert", "[dyamic_small_array]")
{
    SECTION("Fit in internal storage")
    {
        TestObjectCnt objs[5];
        initTestObjectSequence(objs);

        SECTION("Objects are inserted in order")
        {
            {
                DSArrayTest<5> small_vect(sbstd::begin(objs), sbstd::end(objs));

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
            DSArrayTestAlloc stl_alloc{alloc_stats};

            {
                DSArrayTestA<5> small_vect(sbstd::begin(objs), sbstd::end(objs), stl_alloc);

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
                DSArrayTest<5> small_vect(sbstd::begin(objs), sbstd::end(objs));

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
            DSArrayTestAlloc stl_alloc{alloc_stats};

            {
                DSArrayTestA<5> small_vect(sbstd::begin(objs), sbstd::end(objs), stl_alloc);

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

TEST_CASE_METHOD(DSArrayFixture, "DSArray copy ctor", "[dyamic_small_array]")
{
    SECTION("Objects are copied in order and fit in internal storage")
    {
        DSArrayTest<4> src_vec(3);
        initTestObjectSequence(src_vec);
        REQUIRE(src_vec.isSmallStorage());

        REQUIRE(TestObjectCnt::getStats() == 3U);

        {
            DSArrayTest<5> dst_vec(src_vec);

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
        DSArrayTestAlloc src_stl_alloc{src_alloc_stats};

        TestAllocator dst_alloc_stats;
        DSArrayTestAlloc dst_stl_alloc{dst_alloc_stats};

        {
            DSArrayTestA<4> src_vec(3, src_stl_alloc);
            initTestObjectSequence(src_vec);

            REQUIRE(src_vec.isSmallStorage());
            REQUIRE(src_alloc_stats.getStats().alloc_count == 0U);
            REQUIRE_FALSE(src_alloc_stats.owns(src_vec.data()));
            REQUIRE(TestObjectCnt::getStats() == 3U);

            DSArrayTestA<5> dst_vec(src_vec, dst_stl_alloc);

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
        DSArrayTest<10> src_vec(6);
        initTestObjectSequence(src_vec);
        REQUIRE(src_vec.isSmallStorage());

        REQUIRE(TestObjectCnt::getStats() == 6U);

        DSArrayTest<5> dst_vec(src_vec);
        REQUIRE_FALSE(dst_vec.isSmallStorage());

        REQUIRE(TestObjectCnt::getStats() == 12U);

        REQUIRE(dst_vec.size() == src_vec.size());
        REQUIRE(sbstd::equal(begin(src_vec), end(src_vec), begin(dst_vec)));
    }

    SECTION("Objects are copied in order and memory is allocated outside internal storage")
    {
        TestAllocator src_alloc;
        DSArrayTestAlloc src_stl_alloc{src_alloc};

        TestAllocator dst_alloc;
        DSArrayTestAlloc dst_stl_alloc{dst_alloc};

        {
            DSArrayTestA<10> src_vec(6, src_stl_alloc);
            initTestObjectSequence(src_vec);

            REQUIRE(src_vec.isSmallStorage());
            REQUIRE(src_alloc.getStats().alloc_count == 0U);
            REQUIRE_FALSE(src_alloc.owns(src_vec.data()));

            REQUIRE(TestObjectCnt::getStats() == 6U);

            DSArrayTestA<5> dst_vec(src_vec, dst_stl_alloc);

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

TEST_CASE_METHOD(DSArrayFixture, "DSArray move ctor", "[dyamic_small_array]")
{
    SECTION("Objects moved between internal storage")
    {
        DSArrayTest<3> src_vec(3);
        void const * const src_data = src_vec.data();
        initTestObjectSequence(src_vec);

        REQUIRE(src_vec.isSmallStorage());
        REQUIRE(TestObjectCnt::getStats() == 3U);

        DSArrayTest<5> dst_vec(sbstd::move(src_vec));

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
        DSArrayTestAlloc src_alloc{alloc};
        DSArrayTestAlloc dst_alloc{alloc};

        DSArrayTestA<3> src_vec(3, src_alloc);
        void const * const src_data = src_vec.data();
        initTestObjectSequence(src_vec);

        REQUIRE(src_vec.isSmallStorage());
        REQUIRE_FALSE(alloc.owns(src_vec.data()));
        REQUIRE(alloc.getStats().alloc_count == 0U);

        REQUIRE(TestObjectCnt::getStats() == 3U);

        DSArrayTestA<5> dst_vec(sbstd::move(src_vec), dst_alloc);
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
        DSArrayTest<5> src_vec(5);
        void const * const src_data = src_vec.data();
        initTestObjectSequence(src_vec);
        REQUIRE(src_vec.isSmallStorage());

        REQUIRE(TestObjectCnt::getStats() == 5U);

        DSArrayTest<3> dst_vec(sbstd::move(src_vec));
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
            DSArrayTestAlloc src_alloc{test_alloc};
            DSArrayTestAlloc dst_alloc{test_alloc};

            DSArrayTestA<5> src_vec(5, src_alloc);
            void const * const src_data = src_vec.data();
            initTestObjectSequence(src_vec);

            REQUIRE(src_vec.isSmallStorage());
            REQUIRE_FALSE(test_alloc.owns(src_vec.data()));
            REQUIRE(test_alloc.getStats().alloc_count == 0U);

            REQUIRE(TestObjectCnt::getStats() == 5U);

            DSArrayTestA<3> dst_vec(sbstd::move(src_vec), dst_alloc);
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
        DSArrayTest<3> src_vec(10);
        auto const src_data = src_vec.data();
        initTestObjectSequence(src_vec);
        REQUIRE_FALSE(src_vec.isSmallStorage());

        REQUIRE(TestObjectCnt::getStats() == 10U);

        DSArrayTest<5> dst_vec(sbstd::move(src_vec));
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
            DSArrayTestAlloc src_alloc{test_alloc};
            DSArrayTestAlloc dst_alloc{test_alloc};

            DSArrayTestA<3> src_vec(10, src_alloc);
            auto const src_data = src_vec.data();
            initTestObjectSequence(src_vec);

            REQUIRE_FALSE(src_vec.isSmallStorage());
            REQUIRE(test_alloc.owns(src_vec.data()));
            REQUIRE(test_alloc.getStats().alloc_count != 0U);

            REQUIRE(TestObjectCnt::getStats() == 10U);

            DSArrayTestA<5> dst_vec(sbstd::move(src_vec), dst_alloc);
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
        DSArrayTest<5> src_vec(10);
        void const * const src_data = src_vec.data();

        initTestObjectSequence(src_vec);
        REQUIRE_FALSE(src_vec.isSmallStorage());

        REQUIRE(TestObjectCnt::getStats() == 10U);

        DSArrayTest<15> dst_vec(sbstd::move(src_vec));

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
            DSArrayTestAlloc src_alloc{test_alloc};
            DSArrayTestAlloc dst_alloc{test_alloc};

            DSArrayTestA<5> src_vec(10, src_alloc);
            void const * const src_data = src_vec.data();
            initTestObjectSequence(src_vec);

            REQUIRE_FALSE(src_vec.isSmallStorage());
            REQUIRE(test_alloc.owns(src_vec.data()));

            REQUIRE(TestObjectCnt::getStats() == 10U);

            DSArrayTestA<15> dst_vec(sbstd::move(src_vec), dst_alloc);

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

TEST_CASE_METHOD(DSArrayFixture, "DSArray emplace back", "[dyamic_small_array]")
{
    DSArrayTest<5> vect;

    auto const & new_value = vect.emplace_back(0xFFU);
    REQUIRE_FALSE(vect.empty());
    REQUIRE(1U == vect.size());
    REQUIRE(new_value.getId() == 0xFFU);
    REQUIRE(TestObjectCnt::getStats() == 1U);
}

TEST_CASE_METHOD(DSArrayFixture, "DSArray push back", "[dyamic_small_array]")
{
    SECTION("Push lvalue")
    {
        DSArrayTest<5> vect;

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
        DSArrayTest<5> vect;

        vect.push_back({0xFFU});

        REQUIRE_FALSE(vect.empty());
        REQUIRE(1U == vect.size());
        REQUIRE(vect.back().getId() == 0xFFU);

        REQUIRE(TestObjectCnt::getStats() == 1U);
    }
}

TEST_CASE_METHOD(DSArrayFixture, "DSArray clear", "[dyamic_small_array]")
{
    SECTION("Fit in internal storage")
    {
        DSArrayTest<5> vect(3);

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
            DSArrayTestAlloc src_alloc{test_alloc};
            DSArrayTestA<5> vect(10U, src_alloc);
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

TEST_CASE_METHOD(DSArrayFixture, "DSArray sub-script accessors", "[dyamic_small_array]")
{
    DSArrayTest<5> vect{5};
    initTestObjectSequence(vect);

    for (usize idx = 0; idx != 5; ++idx)
    {
        REQUIRE(vect.at(idx).getId() == idx);
        REQUIRE(vect[idx].getId() == idx);
    }

    REQUIRE(vect.front().getId() == 0U);
    REQUIRE(vect.back().getId() == 4U);
}

TEST_CASE_METHOD(DSArrayFixture, "DSArray capacity reservation", "[dyamic_small_array]")
{
    SECTION("Reserving within internal storage does not allocate memory")
    {
        DSArrayTest<5> vect(2);
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
        DSArrayTest<5> vect(2);
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
            DSArrayTestAlloc vect_alloc{test_alloc};
            DSArrayTestA<5> vect(3U, vect_alloc);
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

TEST_CASE_METHOD(DSArrayFixture, "DSArray shrink to fit", "[dyamic_small_array]")
{
    SECTION("Shrink to fit has no effect when fitting in small storage")
    {
        DSArrayTest<5> vect(3);
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
            DSArrayTestAlloc vect_alloc{test_alloc};
            DSArrayTestA<5> vect(vect_alloc);
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
            DSArrayTestAlloc vect_alloc{test_alloc};
            DSArrayTestA<5> vect{3, vect_alloc};
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

TEST_CASE_METHOD(DSArrayFixture, "DSArray pop back", "[dyamic_small_array]")
{
    DSArrayTest<5> vect(3);
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

TEST_CASE_METHOD(DSArrayFixture, "DSArray maximum size", "[dyamic_small_array]")
{
    DSArrayTest<5> vect;
    REQUIRE(sbstd::numeric_limits<DSArrayTest<5>::size_type>::max() == vect.max_size());
}

TEST_CASE_METHOD(DSArrayFixture, "DSArray resize", "[dyamic_small_array]")
{
    SECTION("Resize using the same size does not affect the vector")
    {
        DSArrayTest<5> vect(2U);
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
        DSArrayTest<5> vect(2U);
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
            DSArrayTestAlloc vect_alloc{test_alloc};
            DSArrayTestA<5> vect{2, vect_alloc};
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
            DSArrayTestAlloc vect_alloc{test_alloc};
            DSArrayTestA<5> vect{7, vect_alloc};
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
        DSArrayTest<5> vect(2U);
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

TEST_CASE_METHOD(DSArrayFixture, "DSArray assign value", "[dyamic_small_array]")
{
    SECTION("Assign multiple times an object in small storage")
    {
        TestObjectCnt const REF_OBJECT(0xFFU);

        DSArrayTest<5> vect(3);
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

        DSArrayTest<5> vect(3);
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
            DSArrayTestAlloc vect_alloc{test_alloc};
            DSArrayTestA<5> vect(3, vect_alloc);
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
            DSArrayTestAlloc vect_alloc{test_alloc};
            DSArrayTestA<5> vect(10, vect_alloc);
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

TEST_CASE_METHOD(DSArrayFixture, "DSArray assign operator", "[dyamic_small_array]")
{
    SECTION("Assign a smaller vector which fits in internal storage while using interal storage")
    {
        DSArrayTest<3> vect_src(3);
        initTestObjectSequence(vect_src);

        REQUIRE(TestObjectCnt::getStats() == 3U);
        REQUIRE(vect_src.isSmallStorage());

        DSArrayTest<5> vect(5);
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
            DSArrayTest<5> vect_src(5);
            initTestObjectSequence(vect_src);

            REQUIRE(TestObjectCnt::getStats() == 5U);
            REQUIRE(vect_src.isSmallStorage());

            DSArrayTestAlloc vect_alloc{test_alloc};
            DSArrayTestA<3> vect(3, vect_alloc);
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
            DSArrayTest<3> vect_src(3);
            initTestObjectSequence(vect_src);

            REQUIRE(TestObjectCnt::getStats() == 3U);
            REQUIRE(vect_src.isSmallStorage());

            DSArrayTestAlloc vect_alloc{test_alloc};
            DSArrayTestA<3> vect(10, vect_alloc);
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

TEST_CASE_METHOD(DSArrayFixture, "DSArray swap", "[dyamic_small_array]")
{
    SECTION("Swap heap storage")
    {
        DSArrayTest<3> vect1(10), vect2(5);

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
        DSArrayTest<20> vect1(10), vect2(5);

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
        DSArrayTest<5> vect1(5), vect2(10);

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
        DSArrayTest<5> vect2(5), vect1(10);

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

TEST_CASE_METHOD(DSArrayFixture, "DSArray without allocator has not overhead", "[dyamic_small_array]")
{
    // sizeof(m_begin) + sizeof(m_end) + sizeof(m_storage_end) + sizeof(small storage test = void *)
    // STATIC_REQUIRE(sizeof(DSArray<b8, sizeof(void *)>) == 4 * sizeof(void *));
}

#include <extern_epilog.h>
