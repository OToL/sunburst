
#include <gtest/object_tracker.h>

namespace gtest::ext {

static ObjectTracker::Stats gs_stats = {};

ObjectTracker::ObjectTracker(sb::usize id)
    : m_Id(id)
{
    ++gs_stats.m_alive_obj_count;
}

ObjectTracker::ObjectTracker(ObjectTracker const & src)
    : m_Id(src.m_Id)
{
    ++gs_stats.m_alive_obj_count;
}

ObjectTracker::ObjectTracker(ObjectTracker && src)
    : m_Id(src.m_Id)
{
    ++gs_stats.m_alive_obj_count;
}

ObjectTracker::~ObjectTracker()
{
    --gs_stats.m_alive_obj_count;
}

ObjectTracker & ObjectTracker::operator=(ObjectTracker const & src)
{
    m_Id = src.m_Id;

    return *this;
}

ObjectTracker & ObjectTracker::operator=(ObjectTracker && src)
{
    m_Id = src.m_Id;

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
