
sbAddLibrary(core STATIC
    _impl/string_format.cpp
    _impl/log.cpp
    _impl/platform.cpp
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
    _impl/memory/allocator/incremental_allocator.cpp
    _impl/memory/global_heap.cpp
    _impl/memory/memory_utility.cpp
    _impl/memory/new.cpp)
  
if(SB_TARGET_LINUX)
    target_sources(core 
        PRIVATE
            _pimpl/linux/file_system_linux.cpp
            _pimpl/linux/malloc_linux.cpp)
elseif(SB_TARGET_MACOS)
    target_sources(core 
        PRIVATE
            _pimpl/macos/platform_macos.cpp
            _pimpl/macos/file_system_macos.cpp
            _pimpl/macos/malloc_macos.cpp)
elseif(SB_TARGET_WINDOWS)
    target_sources(core 
        PRIVATE
            _pimpl/windows/platform_win.cpp
            _pimpl/windows/file_system_win.cpp
            _pimpl/windows/malloc_win.cpp)
else()
    message(FATAL_ERROR "sb::core sources not configured for ${SB_TARGET_OS}")
endif()

sbAddLibraryTest(core
    _test/compiler_test.cpp
    _test/small_vector_test.cpp
    _test/conversion_test.cpp
    _test/crc32_test.cpp
    _test/crc64_test.cpp
    _test/string_utility_test.cpp
    _test/enum_test.cpp
    _test/fnv32_test.cpp
    _test/fnv64_test.cpp
    _test/hash_str_test.cpp
    _test/incremental_allocator_test.cpp
    _test/main.cpp
    _test/bitwise_test.cpp
    _test/memory_arena_composer_test.cpp
    _test/new_test.cpp
    _test/null_allocator_test.cpp
    _test/pool_allocator_test.cpp
    _test/ring_buffer_Test.cpp
    _test/static_string_test.cpp
    _test/static_vector_Test.cpp
    _test/string_format_test.cpp
    _test/unique_ptr_test.cpp
    _test/stl_allocator_test.cpp
    _test/typed_id_test.cpp)
        
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    



