include(${SB_CATCH_CMAKE_DIR_PATH}/catch.cmake) 

add_executable(core_test
    bitwise_test.cpp
    compiler_test.cpp
    conversion_test.cpp
    crc32_test.cpp
    crc64_test.cpp
    enum_test.cpp
    fnv1a32_test.cpp
    fnv1a64_test.cpp
    handle_test.cpp
    hash_str_test.cpp
    small_vector_test.cpp
    string_utility_test.cpp
    string_format_test.cpp
    # static_string_test.cpp
    # static_vector_test.cpp
    # alloc_test.cpp
    # incremental_allocator_test.cpp
    # null_allocator_test.cpp
    # pool_allocator_test.cpp
    # ring_buffer_Test.cpp
    # stl_allocator_test.cpp
    # unique_ptr_test.cpp
)

target_link_libraries(core_test
    PRIVATE    
        sbext::catch 
        sb::cxx_properties
        sb::core
        sb::unit)

catch_discover_tests(
    core_test
    TEST_PREFIX
        "core_test."
    EXTRA_ARGS
        -s
        --reporter=xml
        --out=core_test.xml)
