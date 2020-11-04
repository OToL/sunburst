#include <sb_unit/test_object_cnt.h>

#include <sb_core/core.h>
#include <sb_core/error.h>

namespace sb {

static TestObjectCnt::Stats gs_stats;

TestObjectCnt::TestObjectCnt()
    : _valid(true)
    , _id(0U)
{
    ++gs_stats.valid_obj_cnt;
    ++gs_stats.obj_cnt;
}

TestObjectCnt::TestObjectCnt(usize id)
    : _valid(true)
    , _id(id)
{
    ++gs_stats.obj_cnt;
    ++gs_stats.valid_obj_cnt;
}

TestObjectCnt::TestObjectCnt(TestObjectCnt && src)
    : _valid(true)
    , _id(src._id)
{
    sbAssert(src._valid);
    ++gs_stats.obj_cnt;

    src._valid = false;
}

TestObjectCnt::TestObjectCnt(TestObjectCnt const & src)
    : _valid(true)
    , _id(src._id)
{
    ++gs_stats.obj_cnt;
    ++gs_stats.valid_obj_cnt;
}

TestObjectCnt & TestObjectCnt::operator=(TestObjectCnt const & src)
{
    sbAssert(_valid && src._valid);

    _id = src._id;

    return *this;
}

TestObjectCnt & TestObjectCnt::operator=(TestObjectCnt && src)
{
    sbAssert(src._valid);

    _id = src._id;

    if (_valid)
    {
        sbAssert(0 < gs_stats.valid_obj_cnt);
        --gs_stats.valid_obj_cnt;
    }
    else
    {
        _valid = true;
    }

    src._valid = false;

    return *this;
}

TestObjectCnt::~TestObjectCnt()
{
    sbAssert(0 < gs_stats.obj_cnt);
    --gs_stats.obj_cnt;

    if (_valid)
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

bool operator==(TestObjectCnt const & val1, TestObjectCnt const & val2)
{
    return val1.getId() == val2.getId();
}

} // namespace sb
