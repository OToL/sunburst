function(sb_setup_toolchain)
    # Patch default CMAKE_CXX_FLAGS
    # @todo: we could maybe use CMAKE_CXX_FLAGS_INIT set from toolchain initcache
    set(PATCHED_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REGEX REPLACE "/W[0-9]" "" PATCHED_CMAKE_CXX_FLAGS "${PATCHED_CMAKE_CXX_FLAGS}")
    string(REGEX REPLACE "/EH[csar]*" "" PATCHED_CMAKE_CXX_FLAGS "${PATCHED_CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${PATCHED_CMAKE_CXX_FLAGS}" PARENT_SCOPE)
endfunction()

function(sb_set_target_properties BASE_TARGET_NAME)

    if(SB_TARGET_CPU_ARCH_64B) 
        target_compile_definitions(${BASE_TARGET_NAME}_public 
            INTERFACE 
                SB_CPU_64_BIT)
    else()
        message(FATAL_ERROR "Unknown target CPU architecture")
    endif()

    target_compile_features(${BASE_TARGET_NAME}_public INTERFACE cxx_std_20)
    target_compile_definitions(${BASE_TARGET_NAME}_public 
        INTERFACE 
            SB_COMPILER_MSVC 
            SB_PLATFORM_WINDOWS
            _HAS_EXCEPTIONS=0)

endfunction()

function(sb_set_target_warnings BASE_TARGET_NAME)
    set(WARNING_IGNORE_LIST 
        /wd4068 # Unknown pragma
        /wd5045 # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
        /wd4514 # 'function' : unreferenced inline function has been remove
        /wd4710 # 'function' : function not inline
        /wd4820 # 'bytes' bytes padding added after construct 'member_name'
        /wd4577 # 'noexcept' used with no exception handling mode specified; termination on exception is not guaranteed. Specify /EHsc
        )

    if(SB_WARNINGS_AS_ERRORS)
        list(APPEND CONDITIONAL_OPTIONS "/WX")
    endif()

    target_compile_options(${BASE_TARGET_NAME}_private 
        INTERFACE 
            /Wall /permissive- ${WARNING_IGNORE_LIST} ${CONDITIONAL_OPTIONS})
endfunction()