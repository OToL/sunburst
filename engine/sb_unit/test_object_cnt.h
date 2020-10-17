#include <sb_core/core.h>

#include <sb_std/span>

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
        CompEqual(usize id)
            : m_id(id)
        {
        }

        CompEqual(TestObjectCnt const & obj)
            : m_id(obj.getId())
        {
        }

        bool operator()(TestObjectCnt const & obj) const
        {
            return m_id == obj.getId();
        }

        usize m_id = 0U;
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
        return m_id;
    }

    void setId(ValueType id)
    {
        m_id = id;
    }

    static Stats getStats();

    static void resetStats();

    static void initSequence(sbstd::span<TestObjectCnt> objects, ValueType start_id = 0U);

    static bool areSequencial(sbstd::span<TestObjectCnt> objects, ValueType start_id = 0U);

private:
    b8 m_valid;
    ValueType m_id;
};

bool operator==(TestObjectCnt const & lval, TestObjectCnt::ValueType rval);
bool operator==(TestObjectCnt::ValueType lval, TestObjectCnt const & rval);
bool operator==(TestObjectCnt::Stats const & stats, usize val);
bool operator==(TestObjectCnt const & val1, TestObjectCnt const & val2);

} // namespace sb
