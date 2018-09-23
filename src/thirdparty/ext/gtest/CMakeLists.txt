    
sbAddLibrary(gtest_ext STATIC
    _impl/allocator_stats.cpp
    _impl/object_tracker.cpp)

target_link_libraries(gtest_ext
    PUBLIC
        gtest)
