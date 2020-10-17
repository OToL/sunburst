#include <sb_unit/test_object_cnt.h>

#include <sb_core/core.h>
#include <sb_core/error.h>

namespace sb {

static TestObjectCnt::Stats gs_stats;

TestObjectCnt::TestObjectCnt()
    : m_valid(true)
    , m_id(0U)
{
    ++gs_stats.valid_obj_cnt;
    ++gs_stats.obj_cnt;
}

TestObjectCnt::TestObjectCnt(usize id)
    : m_valid(true)
    , m_id(id)
{
    ++gs_stats.obj_cnt;
    ++gs_stats.valid_obj_cnt;
}

TestObjectCnt::TestObjectCnt(TestObjectCnt && src)
    : m_valid(true)
    , m_id(src.m_id)
{
    sbAssert(src.m_valid);
    ++gs_stats.obj_cnt;

    src.m_valid = false;
}

TestObjectCnt::TestObjectCnt(TestObjectCnt const & src)
    : m_valid(true)
    , m_id(src.m_id)
{
    ++gs_stats.obj_cnt;
    ++gs_stats.valid_obj_cnt;
}

TestObjectCnt & TestObjectCnt::operator=(TestObjectCnt const & src)
{
    sbAssert(m_valid && src.m_valid);

    m_id = src.m_id;

    return *this;
}

TestObjectCnt & TestObjectCnt::operator=(TestObjectCnt && src)
{
    sbAssert(src.m_valid);

    m_id = src.m_id;

    if (m_valid)
    {
        sbAssert(0 < gs_stats.valid_obj_cnt);
        --gs_stats.valid_obj_cnt;
    }
    else
    {
        m_valid = true;
    }

    src.m_valid = false;

    return *this;
}

TestObjectCnt::~TestObjectCnt()
{
    sbAssert(0 < gs_stats.obj_cnt);
    --gs_stats.obj_cnt;

    if (m_valid)
    {
        sbAssert(0 < gs_stats.valid_obj_cnt);
        --gs_stats.valid_obj_cnt;
    }
}

void TestObjectCnt::resetStats()
{
    gs_stats = {};
}

TestObjectCnt::Stats TestObjectCnt::getStats()
{
    return gs_stats;
}

void TestObjectCnt::initSequence(sbstd::span<TestObjectCnt> objects, usize start_id)
{
    usize obj_id = start_id;

    for (auto & obj : objects)
    {
        obj.setId(obj_id);
        ++obj_id;
    }
}

bool TestObjectCnt::areSequencial(sbstd::span<TestObjectCnt> objects, usize start_id)
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

bool operator==(TestObjectCnt const & lval, TestObjectCnt::ValueType rval)
{
    return lval.getId() == rval;
}

bool operator==(TestObjectCnt::ValueType lval, TestObjectCnt const & rval)
{
    return lval == rval.getId();
}

bool operator==(TestObjectCnt::Stats const & stats, usize val)
{
    return (stats.obj_cnt == val) && (stats.valid_obj_cnt == val);
}

bool operator==(usize val, TestObjectCnt::Stats const & stats)
{
    return operator==(stats, val);
}

bool operator==(TestObjectCnt const & val1, TestObjectCnt const & val2)
{
    return val1.getId() == val2.getId();
}

} // namespace sb
