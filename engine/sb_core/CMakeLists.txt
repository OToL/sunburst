if(SB_TOOLCHAIN_ID STREQUAL "msvc")
    set(SB_RPMALLOC_WARNINGS "/wd4255 /wd5220 /wd4365 /wd5039 /wd4430 /wd4668")
elseif(SB_TOOLCHAIN_ID STREQUAL "clang")
    set(SB_RPMALLOC_WARNINGS "-Wno-reserved-id-macro -Wno-static-in-inline")
endif()

add_library(sb_core STATIC
    _impl/string_format.cpp
    _impl/log.cpp
    _impl/string/string_conversion.cpp
    _impl/error.cpp
    _impl/hash/crc32.cpp
    _impl/os_${SB_TARGET_PLATFORM_FILE_SUFFIX}.cpp
    _impl/hash/crc64.cpp
    _impl/io/file_input_stream.cpp
    _impl/io/virtual_file_system.cpp
    _impl/io/path.cpp
    _impl/string/string_utility.cpp
    _impl/memory/allocator/global_heap_allocator.cpp
    _impl/memory/allocator/null_allocator.cpp
    _impl/memory/allocator/incremental_allocator_base.cpp
    _impl/memory/allocator/memory_arena_allocator.cpp
    _impl/memory/allocator/pool_allocator_base.cpp
    _impl/memory/allocator/allocator_wrapper.cpp
    _impl/memory/allocator/container_allocator.cpp
    _impl/memory/allocator/container_allocator_wrapper.cpp
    _impl/memory/global_heap.cpp
    _impl/memory/memory.cpp
    _impl/memory/memory_utility.cpp
    _impl/memory/malloc_rpmalloc.cpp
    _impl/memory/memory_system.cpp
    _impl/extern/rpmalloc/rpmalloc.c
    _impl/memory/virtual_memory_backstore_${SB_TARGET_PLATFORM_FILE_SUFFIX}.cpp
    _impl/io/local_file_system_${SB_TARGET_PLATFORM_FILE_SUFFIX}.cpp
    _impl/memory/malloc_${SB_TARGET_PLATFORM_FILE_SUFFIX}.cpp
    _impl/io/local_file_system.cpp)

set_source_files_properties(_impl/extern/rpmalloc/rpmalloc.c 
    PROPERTIES 
        COMPILE_FLAGS ${SB_RPMALLOC_WARNINGS}
        CXX_CLANG_TIDY ""
        C_CLANG_TIDY "")

add_library(sb::core ALIAS sb_core)

target_link_libraries(sb_core 
    PRIVATE
        sb::cxx_common_private
    PUBLIC
        sb::cxx_common_public)

        
