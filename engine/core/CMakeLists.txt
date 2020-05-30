add_library(core STATIC
    _impl/string_format.cpp
    _impl/log.cpp
    _impl/types.cpp
    _impl/conversion.cpp
    _impl/error.cpp
    _impl/hash/crc32.cpp
    _impl/hash/crc64.cpp
    _impl/io/file.cpp
    _impl/io/file_system.cpp
    _impl/io/path.cpp
    _impl/io/local_file_system_layer.cpp
    _impl/string/string_utility.cpp
    _impl/memory/allocator/global_heap_allocator.cpp
    _impl/memory/allocator/null_allocator.cpp
    _impl/memory/provider/memory_arena_provider.cpp
    _impl/memory/provider/global_heap_provider.cpp
    _impl/memory/global_heap.cpp
    _impl/memory/memory.cpp
    _impl/memory/alloc.cpp
    _impl/platform/os_${SB_TARGET_PLATFORM_FILE_SUFFIX}.cpp
    _impl/platform/file_system_${SB_TARGET_PLATFORM_FILE_SUFFIX}.cpp
    _impl/platform/malloc_${SB_TARGET_PLATFORM_FILE_SUFFIX}.cpp)
 
add_library(sb::core ALIAS core)

target_link_libraries(core 
    PRIVATE
        sb::cxx_properties)
