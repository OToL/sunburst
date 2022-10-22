#include <sb_core/core.h>

#include <sb_slw/span>

namespace sb {

class TestObjectCnt
{
public:
    using ValueType = usize;

    struct Stats
    {
        usize obj_cnt = 0U;
        usize valid_obj_cnt = 0U;
    };

    struct CompEqual
    {
        CompEqual(usize val)
            : id(val)
        {
        }

        CompEqual(TestObjectCnt const & obj)
            : id(obj.getId())
        {
        }

        bool operator()(TestObjectCnt const & obj) const
        {
            return id == obj.getId();
        }

        usize id = 0U;
    };

    TestObjectCnt();

    TestObjectCnt(ValueType id);

    TestObjectCnt(TestObjectCnt && src);

    TestObjectCnt(TestObjectCnt const & src);

    TestObjectCnt & operator=(TestObjectCnt const & src);

    TestObjectCnt & operator=(TestObjectCnt && src);

    ~TestObjectCnt();

    ValueType getId() const
    {
        return _id;
    }

    void setId(ValueType id)
    {
        _id = id;
    }

    static Stats getStats();

    static void resetStats();

    static void initSequence(slw::span<TestObjectCnt> objects, ValueType start_id = 0U);

    static bool areSequencial(slw::span<TestObjectCnt> objects, ValueType start_id = 0U);

private:
    b8 _valid = true;
    ValueType _id = 0ULL;
};

bool operator==(TestObjectCnt const & lval, TestObjectCnt::ValueType rval);
bool operator==(TestObjectCnt::ValueType lval, TestObjectCnt const & rval);
bool operator==(TestObjectCnt::Stats const & stats, usize val);
bool operator==(TestObjectCnt const & val1, TestObjectCnt const & val2);

} // namespace sb
