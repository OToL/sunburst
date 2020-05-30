add_library(unit STATIC
    _impl/test_object_cnt.cpp
    _impl/test_allocator.cpp
    _impl/catch_main.cpp)
 
add_library(sb::unit ALIAS unit)

target_link_libraries(unit 
    PRIVATE
        sb::cxx_properties
        sbext::catch
    PUBLIC
        sb::core)
