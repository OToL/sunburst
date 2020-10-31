function(sb_setup_toolchain)

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
            SB_COMPILER_CLANG
            SB_PLATFORM_WINDOWS
            _HAS_EXCEPTIONS=0)

endfunction()

function(sb_set_target_warnings BASE_TARGET_NAME)
    # set(WARNING_IGNORE_LIST 
    #     /wd4068 # Unknown pragma
    #     /wd5045 # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    #     /wd4996 # A function, class member, variable, or typedef that's marked deprecated
    #     /wd4514 # 'function' : unreferenced inline function has been remove
    #     /wd4710 # 'function' : function not inline
    #     /wd4625 # 'derived class' : copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
    #     /wd4521 # 'class' : multiple copy constructors specified
    #     /wd4626 # 'derived class' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
    #     /wd4820 # 'bytes' bytes padding added after construct 'member_name'
    #     /wd4868 # compiler may not enforce left-to-right evaluation order in braced initializer list
    #     /wd4866 # compiler may not enforce left-to-right evaluation order for call to ''
    #     /wd4577 # 'noexcept' used with no exception handling mode specified; termination on exception is not guaranteed. Specify /EHsc
    #     )

    # if(SB_WARNINGS_AS_ERRORS)
    #     list(APPEND CONDITIONAL_OPTIONS "/WX")
    # endif()

    # target_compile_options(${BASE_TARGET_NAME}_private 
    #     INTERFACE 
    #         /Wall /permissive- ${WARNING_IGNORE_LIST} ${CONDITIONAL_OPTIONS})
endfunction()