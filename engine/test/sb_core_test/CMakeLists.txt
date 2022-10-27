include(${SB_CATCH_CMAKE_DIR_PATH}/catch.cmake)

add_executable(sb_core_test
    compiler_test.cpp
    core_test.cpp
    bit_operations_test.cpp
    string_conversion_test.cpp
    crc32_test.cpp
    crc64_test.cpp
    fnv1a32_test.cpp
    fnv1a64_test.cpp
    hash64_str_test.cpp
    hash32_str_test.cpp
    dynamic_small_array_test.cpp
    string_utility_test.cpp
    string_format_test.cpp
    file_hdl_test.cpp
    dynamic_fix_array_test.cpp
    ring_buffer_Test.cpp
    fix_string_test.cpp
    memory_arena_allocator_test.cpp
    incremental_allocator_test.cpp
    null_allocator_test.cpp
    pool_allocator_test.cpp
    global_heap_allocator_test.cpp
    stl_allocator_test.cpp
    ${SB_ENGINE_MEMORY_HOOK_FILE_PATH}
)

target_link_libraries(sb_core_test
    PRIVATE    
        sb::cxx_common_private
        sb::core
        sb::unit)

sb_copy_target_shared_dependencies(sb_core_test)

catch_discover_tests(
    sb_core_test
    TEST_PREFIX
        "core_test."
    EXTRA_ARGS
        -s
        --reporter=xml
        --out=core_test.xml)
