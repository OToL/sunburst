
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
    _impl/memory/allocator/arena_mem_provider.cpp
    _impl/memory/allocator/null_allocator.cpp
    _impl/memory/global_heap.cpp
    _impl/memory/memory.cpp
    _impl/memory/alloc.cpp)
  
if(SB_TARGET_LINUX)
    target_sources(core 
        PRIVATE
            _impl/linux/platform_linux.cpp
            _impl/linux/file_system_linux.cpp
            _impl/linux/malloc_linux.cpp)
elseif(SB_TARGET_MACOS)
    target_sources(core 
        PRIVATE
            _impl/macos/platform_macos.cpp
            _impl/macos/file_system_macos.cpp
            _impl/macos/malloc_macos.cpp)
elseif(SB_TARGET_WINDOWS)
    target_sources(core 
        PRIVATE
            _impl/windows/platform_win.cpp
            _impl/windows/file_system_win.cpp
            _impl/windows/malloc_win.cpp)
else()
    message(FATAL_ERROR "sb::core sources not configured for ${SB_TARGET_OS}")
endif()

        
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    



