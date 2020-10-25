include(${SB_CATCH_CMAKE_DIR_PATH}/catch.cmake) 

add_executable(sb_core_test
    core_test.cpp
    bit_test.cpp
    conversion_test.cpp
    crc32_test.cpp
    crc64_test.cpp
    enum_test.cpp
    fnv1a32_test.cpp
    fnv1a64_test.cpp
    hash64_str_test.cpp
    hash32_str_test.cpp
    small_array_test.cpp
    string_utility_test.cpp
    string_format_test.cpp
    file_hdl_test.cpp
    fix_array_test.cpp
    ring_buffer_Test.cpp
    fix_string_test.cpp
    memory_arena_allocator_test.cpp
    # incremental_allocator_test.cpp
    # alloc_test.cpp
    # null_allocator_test.cpp
    # pool_allocator_test.cpp
    # stl_allocator_test.cpp
)

target_link_libraries(sb_core_test
    PRIVATE    
        sb::cxx_properties_private
        sb::core
        sb::unit)

catch_discover_tests(
    sb_core_test
    TEST_PREFIX
        "core_test."
    EXTRA_ARGS
        -s
        --reporter=xml
        --out=core_test.xml)
