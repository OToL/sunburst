add_library(sb_unit STATIC
    _impl/test_object_cnt.cpp
    _impl/test_allocator.cpp
    _impl/catch_main.cpp)
 
add_library(sb::unit ALIAS sb_unit)

target_link_libraries(sb_unit 
    PRIVATE
        sb::cxx_common_private
    PUBLIC
        sbext::catch
        sb::core
        sb::cxx_common_public)
