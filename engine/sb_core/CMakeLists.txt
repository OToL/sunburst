add_library(sb_core STATIC
    _impl/string_format.cpp
    _impl/log.cpp
    _impl/conversion.cpp
    _impl/error.cpp
    _impl/hash/crc32.cpp
    _impl/hash/crc64.cpp
    _impl/io/file_stream.cpp
    _impl/io/virtual_file_system.cpp
    _impl/io/path.cpp
    _impl/string/string_utility.cpp
    _impl/memory/allocator/global_heap_allocator.cpp
    _impl/memory/allocator/null_allocator.cpp
    _impl/memory/allocator/incremental_allocator_base.cpp
    _impl/memory/allocator/memory_arena_allocator.cpp
    _impl/memory/allocator/pool_allocator_base.cpp
    _impl/memory/global_heap.cpp
    _impl/memory/memory.cpp
    _impl/memory/alloc.cpp
    _impl/api/${SB_TARGET_PLATFORM_ID}/os_${SB_TARGET_PLATFORM_FILE_SUFFIX}.cpp
    _impl/api/${SB_TARGET_PLATFORM_ID}/local_file_system_${SB_TARGET_PLATFORM_FILE_SUFFIX}.cpp
    _impl/api/${SB_TARGET_PLATFORM_ID}/malloc_${SB_TARGET_PLATFORM_FILE_SUFFIX}.cpp)
 
add_library(sb::core ALIAS sb_core)

target_link_libraries(sb_core 
    PRIVATE
        sb::cxx_properties_private
    PUBLIC
        sb::cxx_properties_public)
