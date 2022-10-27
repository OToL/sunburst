function(sb_setup_toolchain)

endfunction()

function(sb_setup_toolchain_properties BASE_TARGET_NAME)

    if(SB_TARGET_CPU_ARCH_64B) 
        target_compile_definitions(${BASE_TARGET_NAME}_public 
            INTERFACE 
                SB_CPU_64_BIT)
    else()
        message(FATAL_ERROR "Unknown target CPU architecture")
    endif()

    target_compile_features(${BASE_TARGET_NAME}_public INTERFACE cxx_std_23)
    target_compile_definitions(${BASE_TARGET_NAME}_public 
        INTERFACE 
            SB_COMPILER_CLANG
            SB_PLATFORM_WINDOWS)
    target_compile_options(${BASE_TARGET_NAME}_public
        INTERFACE
            -fno-exceptions
            -Wno-deprecated-declarations)

endfunction()

function(sb_setup_toolchain_warnings BASE_TARGET_NAME)
    set(WARNING_IGNORE_LIST 
        # TODO: fix later
        -Wno-sign-conversion
        -Wno-exit-time-destructors
        -Wno-global-constructors

        # Don't care
        -Wno-unused-macros
        -Wno-extra-semi-stmt
        -Wno-old-style-cast
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-gnu-zero-variadic-macro-arguments
        -Wno-reserved-identifier
        -Wno-declaration-after-statement
        -Wno-c++20-compat
        -Wno-unknown-warning-option
        -Wno-deprecated-copy-with-dtor
        -Wno-nested-anon-types
        -Wno-gnu-anonymous-struct
        -Wno-missing-prototypes)

    if(SB_ENABLE_WARNING_AS_ERROR)
        list(APPEND CONDITIONAL_OPTIONS "-Werror")
    endif()

    target_compile_options(${BASE_TARGET_NAME}_private 
        INTERFACE 
            -Wall -Wextra -Weverything -pedantic ${WARNING_IGNORE_LIST} ${CONDITIONAL_OPTIONS})
endfunction()

function(sb_setup_toolchain_sanitizers BASE_TARGET_NAME)

endfunction()
