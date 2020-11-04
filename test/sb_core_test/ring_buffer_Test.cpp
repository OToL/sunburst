#include <sb_core/container/ring_buffer.h>
#include <sb_core/memory/allocator/container_allocator_wrapper.h>

#include <sb_unit/test_object_cnt.h>
#include <sb_unit/test_allocator.h>

#include <sb_std/xutility>

#include <catch2/test_prolog.h>
#include <catch2/xcatch.hpp>

using namespace sb;

using TestRingBufferPODA = RingBuffer<usize, ContainerAllocatorWrapper>;
using TestRingBufferA = RingBuffer<TestObjectCnt, ContainerAllocatorWrapper>;
using TestRingBufferPOD = RingBuffer<usize>;
using TestRingBuffer = RingBuffer<TestObjectCnt>;

constexpr usize TEST_RING_BUFFER_CAPACITY = 10;
constexpr usize TEST_RING_BUFFER_REF[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

void FillTestRingBufferPutOverflow(TestRingBufferPOD & test_ring)
{
    for (usize idx = 0; idx != test_ring.capacity(); ++idx)
    {
        test_ring.put_overflow(idx);
    }
}

void FillTestRingBufferPutOverflow(TestRingBuffer & test_ring)
{
    for (usize idx = 0; idx != test_ring.capacity(); ++idx)
    {
        test_ring.put_overflow(idx);
    }
}

void FillTestRingBufferEmplacePutOverflow(TestRingBufferPOD & test_ring)
{
    for (usize idx = 0; idx != test_ring.capacity(); ++idx)
    {
        test_ring.emplace_put_overflow(idx);
    }
}

void FillTestRingBufferEmplacePutOverflow(TestRingBuffer & test_ring)
{
    for (usize idx = 0; idx != test_ring.capacity(); ++idx)
    {
        test_ring.emplace_put_overflow(idx);
    }
}

void FillTestRingBufferPut(TestRingBufferPOD & test_ring)
{
    for (usize idx = 0; idx != test_ring.capacity(); ++idx)
    {
        test_ring.put(idx);
    }
}

void FillTestRingBufferPut(TestRingBuffer & test_ring)
{
    for (usize idx = 0; idx != test_ring.capacity(); ++idx)
    {
        test_ring.put(idx);
    }
}

void FillTestRingBufferEmplacePut(TestRingBufferPOD & test_ring)
{
    for (usize idx = 0; idx != test_ring.capacity(); ++idx)
    {
        test_ring.emplace_put(idx);
    }
}

void FillTestRingBufferEmplacePut(TestRingBuffer & test_ring)
{
    for (usize idx = 0; idx != test_ring.capacity(); ++idx)
    {
        test_ring.emplace_put(idx);
    }
}

TEST_CASE("RingBuffer ctor", "[ring_buffer]")
{
    TestObjectCnt::resetStats();
    TestAllocator alloc_stats;

    TestRingBufferA ring_buffer(TEST_RING_BUFFER_CAPACITY, ContainerAllocatorWrapper(alloc_stats));

    REQUIRE_FALSE(ring_buffer.full());
    REQUIRE(ring_buffer.empty());
    REQUIRE(ring_buffer.size() == 0U);
    REQUIRE(ring_buffer.capacity() == TEST_RING_BUFFER_CAPACITY);
    REQUIRE(TestObjectCnt::getStats() == 0U);
    REQUIRE(alloc_stats.getStats().alloc_count == 1U);
    REQUIRE(alloc_stats.getStats().allocated_byte != 0U);
}

TEST_CASE("RingBuffer put_overflow", "[ring_buffer]")
{
    SECTION("RingBuffer POD filling")
    {
        TestRingBufferPOD ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferPutOverflow(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for (usize ref_val : TEST_RING_BUFFER_REF)
        {            
            REQUIRE(ref_val == ring_buffer.pop());
        }

        REQUIRE(ring_buffer.size() == 0U);
        REQUIRE(ring_buffer.empty());

    }

    constexpr usize OVERFLOW_REF[] = {10, 11, 12}; 

    SECTION("RingBuffer POD overflow")
    {
        TestRingBufferPOD ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferPutOverflow(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for (usize val : OVERFLOW_REF)
        {
            ring_buffer.put_overflow(val);
        }

        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for (usize idx = 0 ; idx != (TEST_RING_BUFFER_CAPACITY - sbstd::size(OVERFLOW_REF)) ; ++idx)
        {
            REQUIRE(ring_buffer.pop() == TEST_RING_BUFFER_REF[idx + sbstd::size(OVERFLOW_REF)]);
        }

        REQUIRE(sbstd::size(OVERFLOW_REF) == ring_buffer.size());

        for (usize idx = 0 ; idx != sbstd::size(OVERFLOW_REF) ; ++idx)
        {
            REQUIRE(ring_buffer.pop() == OVERFLOW_REF[idx]);
        }

        REQUIRE(ring_buffer.empty());
    }

    SECTION("RingBuffer non-POD fill")
    {
        TestObjectCnt::resetStats();
        TestRingBuffer ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferPutOverflow(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == TEST_RING_BUFFER_CAPACITY);

        for (usize ref_val : TEST_RING_BUFFER_REF)
        {            
            REQUIRE(ref_val == ring_buffer.pop());
        }

        REQUIRE(TestObjectCnt::getStats() == 0);
        REQUIRE(ring_buffer.size() == 0U);
        REQUIRE(ring_buffer.empty());

    }

    SECTION("RingBuffer non-POD overflow")
    {
        TestObjectCnt::resetStats();
        TestRingBuffer ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferPutOverflow(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.capacity());

        for (usize val : OVERFLOW_REF)
        {
            ring_buffer.put_overflow(val);
        }

        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.capacity());

        for (usize idx = 0 ; idx != (TEST_RING_BUFFER_CAPACITY - sbstd::size(OVERFLOW_REF)) ; ++idx)
        {
            REQUIRE(ring_buffer.pop() == TEST_RING_BUFFER_REF[idx + sbstd::size(OVERFLOW_REF)]);
        }

        REQUIRE(TestObjectCnt::getStats() == ring_buffer.size());
        REQUIRE(sbstd::size(OVERFLOW_REF) == ring_buffer.size());

        for (usize idx = 0 ; idx != sbstd::size(OVERFLOW_REF) ; ++idx)
        {
            REQUIRE(ring_buffer.pop() == OVERFLOW_REF[idx]);
        }

        REQUIRE(ring_buffer.empty());
        REQUIRE(TestObjectCnt::getStats() == 0U);
    }
}

TEST_CASE("RingBuffer emplace_put_overflow", "[ring_buffer]")
{
    SECTION("RingBuffer POD filling")
    {
        TestRingBufferPOD ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferEmplacePutOverflow(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for (usize ref_val : TEST_RING_BUFFER_REF)
        {            
            REQUIRE(ref_val == ring_buffer.pop());
        }

        REQUIRE(ring_buffer.size() == 0U);
        REQUIRE(ring_buffer.empty());

    }

    constexpr usize OVERFLOW_REF[] = {10, 11, 12}; 

    SECTION("RingBuffer POD overflow")
    {
        TestRingBufferPOD ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferEmplacePutOverflow(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for (usize val : OVERFLOW_REF)
        {
            ring_buffer.emplace_put_overflow(val);
        }

        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for (usize idx = 0 ; idx != (TEST_RING_BUFFER_CAPACITY - sbstd::size(OVERFLOW_REF)) ; ++idx)
        {
            REQUIRE(ring_buffer.pop() == TEST_RING_BUFFER_REF[idx + sbstd::size(OVERFLOW_REF)]);
        }

        REQUIRE(sbstd::size(OVERFLOW_REF) == ring_buffer.size());

        for (usize idx = 0 ; idx != sbstd::size(OVERFLOW_REF) ; ++idx)
        {
            REQUIRE(ring_buffer.pop() == OVERFLOW_REF[idx]);
        }

        REQUIRE(ring_buffer.empty());
    }

    SECTION("RingBuffer non-POD fill")
    {
        TestObjectCnt::resetStats();
        TestRingBuffer ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferEmplacePutOverflow(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == TEST_RING_BUFFER_CAPACITY);

        for (usize ref_val : TEST_RING_BUFFER_REF)
        {            
            REQUIRE(ref_val == ring_buffer.pop());
        }

        REQUIRE(TestObjectCnt::getStats() == 0);
        REQUIRE(ring_buffer.size() == 0U);
        REQUIRE(ring_buffer.empty());

    }

    SECTION("RingBuffer non-POD overflow")
    {
        TestObjectCnt::resetStats();
        TestRingBuffer ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferEmplacePutOverflow(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.capacity());

        for (usize val : OVERFLOW_REF)
        {
            ring_buffer.emplace_put_overflow(val);
        }

        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.capacity());

        for (usize idx = 0 ; idx != (TEST_RING_BUFFER_CAPACITY - sbstd::size(OVERFLOW_REF)) ; ++idx)
        {
            REQUIRE(ring_buffer.pop() == TEST_RING_BUFFER_REF[idx + sbstd::size(OVERFLOW_REF)]);
        }

        REQUIRE(TestObjectCnt::getStats() == ring_buffer.size());
        REQUIRE(sbstd::size(OVERFLOW_REF) == ring_buffer.size());

        for (usize idx = 0 ; idx != sbstd::size(OVERFLOW_REF) ; ++idx)
        {
            REQUIRE(ring_buffer.pop() == OVERFLOW_REF[idx]);
        }

        REQUIRE(ring_buffer.empty());
        REQUIRE(TestObjectCnt::getStats() == 0U);
    }
}

TEST_CASE("RingBuffer put", "[ring_buffer]")
{
    SECTION("RingBuffer POD filling")
    {
        TestRingBufferPOD ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferPut(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for (usize ref_val : TEST_RING_BUFFER_REF)
        {            
            REQUIRE(ref_val == ring_buffer.pop());
        }

        REQUIRE(ring_buffer.size() == 0U);
        REQUIRE(ring_buffer.empty());

    }

    constexpr usize OVERFLOW_REF[] = {10, 11, 12}; 

    SECTION("RingBuffer POD overflow")
    {
        TestRingBufferPOD ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferPut(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for (usize val : OVERFLOW_REF)
        {
            REQUIRE_FALSE(ring_buffer.put(val));
        }

        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for (usize idx = 0 ; idx != TEST_RING_BUFFER_CAPACITY ; ++idx)
        {
            REQUIRE(ring_buffer.pop() == TEST_RING_BUFFER_REF[idx]);
        }

        REQUIRE(ring_buffer.empty());
    }

    SECTION("RingBuffer non-POD fill")
    {
        TestObjectCnt::resetStats();
        TestRingBuffer ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferPut(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == TEST_RING_BUFFER_CAPACITY);

        for (usize ref_val : TEST_RING_BUFFER_REF)
        {            
            REQUIRE(ref_val == ring_buffer.pop());
        }

        REQUIRE(TestObjectCnt::getStats() == 0);
        REQUIRE(ring_buffer.size() == 0U);
        REQUIRE(ring_buffer.empty());

    }

    SECTION("RingBuffer non-POD overflow")
    {
        TestObjectCnt::resetStats();
        TestRingBuffer ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferPut(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.capacity());

        for (usize val : OVERFLOW_REF)
        {
            REQUIRE_FALSE(ring_buffer.put(val));
        }

        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.capacity());

        for (usize idx = 0 ; idx != TEST_RING_BUFFER_CAPACITY ; ++idx)
        {
            REQUIRE(ring_buffer.pop() == TEST_RING_BUFFER_REF[idx]);
        }

        REQUIRE(ring_buffer.empty());
        REQUIRE(TestObjectCnt::getStats() == 0U);
    }
}

TEST_CASE("RingBuffer emplace_put", "[ring_buffer]")
{
    SECTION("RingBuffer POD filling")
    {
        TestRingBufferPOD ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferEmplacePut(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for (usize ref_val : TEST_RING_BUFFER_REF)
        {            
            REQUIRE(ref_val == ring_buffer.pop());
        }

        REQUIRE(ring_buffer.size() == 0U);
        REQUIRE(ring_buffer.empty());

    }

    constexpr usize OVERFLOW_REF[] = {10, 11, 12}; 

    SECTION("RingBuffer POD overflow")
    {
        TestRingBufferPOD ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferEmplacePut(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for (usize val : OVERFLOW_REF)
        {
            REQUIRE_FALSE(ring_buffer.emplace_put(val));
        }

        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for (usize idx = 0 ; idx != TEST_RING_BUFFER_CAPACITY ; ++idx)
        {
            REQUIRE(ring_buffer.pop() == TEST_RING_BUFFER_REF[idx]);
        }

        REQUIRE(ring_buffer.empty());
    }

    SECTION("RingBuffer non-POD fill")
    {
        TestObjectCnt::resetStats();
        TestRingBuffer ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferEmplacePut(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == TEST_RING_BUFFER_CAPACITY);

        for (usize ref_val : TEST_RING_BUFFER_REF)
        {            
            REQUIRE(ref_val == ring_buffer.pop());
        }

        REQUIRE(TestObjectCnt::getStats() == 0);
        REQUIRE(ring_buffer.size() == 0U);
        REQUIRE(ring_buffer.empty());

    }

    SECTION("RingBuffer non-POD overflow")
    {
        TestObjectCnt::resetStats();
        TestRingBuffer ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferEmplacePut(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.capacity());

        for (usize val : OVERFLOW_REF)
        {
            REQUIRE_FALSE(ring_buffer.emplace_put(val));
        }

        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.capacity());

        for (usize idx = 0 ; idx != TEST_RING_BUFFER_CAPACITY ; ++idx)
        {
            REQUIRE(ring_buffer.pop() == TEST_RING_BUFFER_REF[idx]);
        }

        REQUIRE(ring_buffer.empty());
        REQUIRE(TestObjectCnt::getStats() == 0U);
    }
}

TEST_CASE("RingBuffer dtor cleanup", "[ring_buffer]")
{
    TestObjectCnt::resetStats();

    {
        TestRingBuffer ring_buffer(TEST_RING_BUFFER_CAPACITY);

        ring_buffer.put(1);
        ring_buffer.put(2);

        REQUIRE(TestObjectCnt::getStats() == 2U);
        REQUIRE(ring_buffer.size() == 2U);
    }

    REQUIRE(TestObjectCnt::getStats() == 0U);
}

TEST_CASE("RingBuffer pop after full", "[ring_buffer]")
{
    constexpr usize TEST_VALUE = 100U;

    SECTION("RingBuffer POD put")
    {
        TestRingBufferPOD ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferPut(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        REQUIRE_FALSE(ring_buffer.put(TEST_VALUE));
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        REQUIRE(TEST_RING_BUFFER_REF[0] == ring_buffer.pop());
        REQUIRE(ring_buffer.size() == (ring_buffer.capacity() - 1));

        REQUIRE(ring_buffer.put(TEST_VALUE));
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for(usize idx = 1 ; idx != TEST_RING_BUFFER_CAPACITY; ++idx)
        {
            REQUIRE(TEST_RING_BUFFER_REF[idx] == ring_buffer.pop());
        }
        REQUIRE(ring_buffer.size() == 1U);

        REQUIRE(TEST_VALUE == ring_buffer.pop());
        REQUIRE(ring_buffer.empty());
    }

    SECTION("RingBuffer POD emplace_put")
    {
        TestRingBufferPOD ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferEmplacePut(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        REQUIRE_FALSE(ring_buffer.emplace_put(TEST_VALUE));
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        REQUIRE(TEST_RING_BUFFER_REF[0] == ring_buffer.pop());
        REQUIRE(ring_buffer.size() == (ring_buffer.capacity() - 1));

        REQUIRE(ring_buffer.emplace_put(TEST_VALUE));
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for(usize idx = 1 ; idx != TEST_RING_BUFFER_CAPACITY; ++idx)
        {
            REQUIRE(TEST_RING_BUFFER_REF[idx] == ring_buffer.pop());
        }
        REQUIRE(ring_buffer.size() == 1U);

        REQUIRE(TEST_VALUE == ring_buffer.pop());
        REQUIRE(ring_buffer.empty());
    }

    SECTION("RingBuffer POD emplace_put_overflow")
    {
        TestRingBufferPOD ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferEmplacePutOverflow(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        REQUIRE(TEST_RING_BUFFER_REF[0] == ring_buffer.pop());
        REQUIRE(ring_buffer.size() == (ring_buffer.capacity() - 1));

        ring_buffer.emplace_put_overflow(TEST_VALUE);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());

        for(usize idx = 1 ; idx != TEST_RING_BUFFER_CAPACITY; ++idx)
        {
            REQUIRE(TEST_RING_BUFFER_REF[idx] == ring_buffer.pop());
        }
        REQUIRE(ring_buffer.size() == 1U);

        REQUIRE(TEST_VALUE == ring_buffer.pop());
        REQUIRE(ring_buffer.empty());
    }

    SECTION("RingBuffer non-POD put")
    {
        TestObjectCnt::resetStats();
        TestRingBuffer ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferPut(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.size());

        REQUIRE_FALSE(ring_buffer.put(TEST_VALUE));
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.capacity());

        REQUIRE(TEST_RING_BUFFER_REF[0] == ring_buffer.pop());
        REQUIRE(ring_buffer.size() == (ring_buffer.capacity() - 1));
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.size());

        REQUIRE(ring_buffer.put(TEST_VALUE));
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.size());

        for(usize idx = 1 ; idx != TEST_RING_BUFFER_CAPACITY; ++idx)
        {
            REQUIRE(TEST_RING_BUFFER_REF[idx] == ring_buffer.pop());
        }
        REQUIRE(ring_buffer.size() == 1U);
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.size());

        REQUIRE(TEST_VALUE == ring_buffer.pop());
        REQUIRE(ring_buffer.empty());
        REQUIRE(TestObjectCnt::getStats() == 0U);
    }

    SECTION("RingBuffer non-POD emplace_put")
    {
        TestObjectCnt::resetStats();
        TestRingBuffer ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferEmplacePut(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.size());

        REQUIRE_FALSE(ring_buffer.emplace_put(TEST_VALUE));
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.capacity());

        REQUIRE(TEST_RING_BUFFER_REF[0] == ring_buffer.pop());
        REQUIRE(ring_buffer.size() == (ring_buffer.capacity() - 1));
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.size());

        REQUIRE(ring_buffer.emplace_put(TEST_VALUE));
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.size());

        for(usize idx = 1 ; idx != TEST_RING_BUFFER_CAPACITY; ++idx)
        {
            REQUIRE(TEST_RING_BUFFER_REF[idx] == ring_buffer.pop());
        }
        REQUIRE(ring_buffer.size() == 1U);
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.size());

        REQUIRE(TEST_VALUE == ring_buffer.pop());
        REQUIRE(ring_buffer.empty());
        REQUIRE(TestObjectCnt::getStats() == 0U);
    }

    SECTION("RingBuffer non-POD emplace_put_overflow")
    {
        TestObjectCnt::resetStats();
        TestRingBuffer ring_buffer(TEST_RING_BUFFER_CAPACITY);

        FillTestRingBufferEmplacePutOverflow(ring_buffer);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.size());

        REQUIRE(TEST_RING_BUFFER_REF[0] == ring_buffer.pop());
        REQUIRE(ring_buffer.size() == (ring_buffer.capacity() - 1));
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.size());

        ring_buffer.emplace_put_overflow(TEST_VALUE);
        REQUIRE(ring_buffer.size() == ring_buffer.capacity());
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.size());

        for(usize idx = 1 ; idx != TEST_RING_BUFFER_CAPACITY; ++idx)
        {
            REQUIRE(TEST_RING_BUFFER_REF[idx] == ring_buffer.pop());
        }
        REQUIRE(ring_buffer.size() == 1U);
        REQUIRE(TestObjectCnt::getStats() == ring_buffer.size());

        REQUIRE(TEST_VALUE == ring_buffer.pop());
        REQUIRE(ring_buffer.empty());
        REQUIRE(TestObjectCnt::getStats() == 0U);
    }
}

#include <catch2/test_epilog.h>
