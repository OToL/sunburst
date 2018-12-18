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

    sb::usize getIdx() const {return m_Idx;}

    void setIdx(sb::usize idx) {m_Idx = idx;}

    struct Stats
    {
        sb::usize m_alive_obj_count;
    };

    static void resetStats();
    static Stats getStats();

private:

    sb::usize m_Idx;
};

}
