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
            SB_COMPILER_CLANG
            SB_PLATFORM_WINDOWS
            _ITERATOR_DEBUG_LEVEL=0)
    target_compile_options(${BASE_TARGET_NAME}_public
        INTERFACE
            -fno-exceptions
            -Wno-deprecated-declarations)

endfunction()

function(sb_setup_toolchain_warnings BASE_TARGET_NAME)
    set(WARNING_IGNORE_LIST 
        -Wno-sign-conversion
        -Wno-reserved-id-macro
        -Wno-extra-semi-stmt
        -Wno-old-style-cast
        -Wno-nonportable-system-include-path
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-gnu-zero-variadic-macro-arguments
        -Wno-exit-time-destructors
        -Wno-global-constructors
        -Wno-missing-prototypes
    )

    if(SB_ENABLE_WARNING_AS_ERROR)
        list(APPEND CONDITIONAL_OPTIONS "-Werror")
    endif()

    target_compile_options(${BASE_TARGET_NAME}_private 
        INTERFACE 
            -Wall -Wextra -Weverything -pedantic -fsanitize=undefined ${WARNING_IGNORE_LIST} ${CONDITIONAL_OPTIONS})
    target_link_options(${BASE_TARGET_NAME}_private 
        INTERFACE 
            -fsanitize=undefined)
    target_link_libraries(${BASE_TARGET_NAME}_private 
        INTERFACE 
            clang_rt.asan_dynamic-x86_64.lib)
endfunction()

function(sb_setup_toolchain BASE_TARGET_NAME)



endfunction()
