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
            SB_PLATFORM_WINDOWS)
    target_compile_options(${BASE_TARGET_NAME}_public
        INTERFACE
            -fno-exceptions
            -Wno-deprecated-declarations)

endfunction()

function(sb_set_target_warnings BASE_TARGET_NAME)
    set(WARNING_IGNORE_LIST 
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-old-style-cast
        -Wno-gnu-zero-variadic-macro-arguments
        -Wno-extra-semi-stmt
        -Wno-sign-conversion
        -Wno-exit-time-destructors
        -Wno-reserved-id-macro
        -Wno-global-constructors
        -Wno-missing-prototypes
        -Wno-nonportable-system-include-path
    )

    if(SB_WARNINGS_AS_ERRORS)
        list(APPEND CONDITIONAL_OPTIONS "-Werror")
    endif()

    target_compile_options(${BASE_TARGET_NAME}_private 
        INTERFACE 
            -Wall -Wextra -Weverything -pedantic ${WARNING_IGNORE_LIST} ${CONDITIONAL_OPTIONS})
endfunction()