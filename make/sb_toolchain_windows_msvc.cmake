function(sb_setup_toolchain)
    # Patch default CMAKE_CXX_FLAGS
    # @todo: we could maybe use CMAKE_CXX_FLAGS_INIT set from toolchain initcache
    set(PATCHED_CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

    if (SB_ENABLE_SANITIZER_ADDRESS)
        # /RTCx is not compatible with -fsanitize 
        string(REGEX REPLACE "/RTC[1csu]*" "" PATCHED_CMAKE_CXX_FLAGS_DEBUG "${PATCHED_CMAKE_CXX_FLAGS_DEBUG}")
    endif()

    set(CMAKE_CXX_FLAGS_DEBUG "${PATCHED_CMAKE_CXX_FLAGS_DEBUG}" PARENT_SCOPE)
endfunction()

function(sb_setup_toolchain_properties BASE_TARGET_NAME)

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
            SB_PLATFORM_WINDOWS)
endfunction()

function(sb_setup_toolchain_warnings BASE_TARGET_NAME)
    set(WARNING_IGNORE_LIST 
        /wd4068 # Unknown pragma
        /wd4514 # 'function' : unreferenced inline function has been remove
        /wd4710 # 'function' : function not inline
        /wd4820 # 'bytes' bytes padding added after construct 'member_name'
        /wd5045 # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
        )

    if(SB_ENABLE_WARNING_AS_ERROR)
        list(APPEND CONDITIONAL_OPTIONS "/WX")
    endif()

    target_compile_options(${BASE_TARGET_NAME}_private 
        INTERFACE 
            /Wall /permissive- ${WARNING_IGNORE_LIST} ${CONDITIONAL_OPTIONS})
endfunction()

function(sb_setup_toolchain_sanitizers BASE_TARGET_NAME)

    if(SB_ENABLE_SANITIZER_ADDRESS)
        list(APPEND SANITIZERS_LIST "address")
    endif()

    if(SANITIZERS_LIST)
        if(NOT TARGET sbext::asan)
            message(FATAL_ERROR "Address sanitizers has not been properly setup")
        endif()
    
        list(JOIN SANITIZERS_LIST "," SANITIZERS_PARAM) 

        target_compile_options(${BASE_TARGET_NAME}_private 
            INTERFACE
                /fsanitize=${SANITIZERS_PARAM})

        target_link_libraries(${BASE_TARGET_NAME}_public
            INTERFACE
                sbext::asan)
    endif()

endfunction()
