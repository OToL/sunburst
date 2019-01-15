#pragma once

#include <core/platform.h>

namespace gtest::ext {

class ObjectTracker
{
public:
    explicit ObjectTracker(sb::usize idx = 0ULL);

    ObjectTracker(ObjectTracker const & src);

    ObjectTracker(ObjectTracker && src);

    virtual ~ObjectTracker();

    ObjectTracker & operator=(ObjectTracker const & src);

    ObjectTracker & operator=(ObjectTracker && src);

    sb::usize getId() const {return m_Id;}

    void setId(sb::usize id) {m_Id = id;}

    struct Stats
    {
        sb::usize m_alive_obj_count;
    };

    static void resetStats();
    static Stats getStats();

private:

    sb::usize m_Id;
};

inline bool operator == (ObjectTracker const & obj1, ObjectTracker const & obj2)
{
    return obj1.getId() == obj2.getId();
}

}
