
#include <gtest/object_tracker.h>

namespace gtest::ext {

static ObjectTracker::Stats gs_stats = {};

ObjectTracker::ObjectTracker(sb::usize idx)
    : m_Idx(idx)
{
    ++gs_stats.m_alive_obj_count;
}

ObjectTracker::ObjectTracker(ObjectTracker const & src)
    : m_Idx(src.m_Idx)
{
    ++gs_stats.m_alive_obj_count;
}

ObjectTracker::ObjectTracker(ObjectTracker && src)
    : m_Idx(src.m_Idx)
{
    ++gs_stats.m_alive_obj_count;
}

ObjectTracker::~ObjectTracker()
{
    --gs_stats.m_alive_obj_count;
}

ObjectTracker & ObjectTracker::operator=(ObjectTracker const & src)
{
    m_Idx = src.m_Idx;

    return *this;
}

ObjectTracker & ObjectTracker::operator=(ObjectTracker && src)
{
    m_Idx = src.m_Idx;

    return *this;
}

ObjectTracker::Stats ObjectTracker::getStats() 
{
    return gs_stats;
}

void ObjectTracker::resetStats()
{
    gs_stats = {};
}

} // namespace wgtest
