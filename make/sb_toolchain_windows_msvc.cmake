function(sb_set_target_common_public_properties TARGET_NAME)

    if(SB_TARGET_CPU_ARCH_64B) 
        target_compile_definitions(${TARGET_NAME} 
            INTERFACE 
                SB_TARGET_CPU_64_BIT)
    else()
        message(FATAL_ERROR "Unknown target CPU architecture")
    endif()

    target_compile_definitions(${TARGET_NAME} 
        INTERFACE 
            SB_COMPILER_MSVC SB_TARGET_WINDOWS)

endfunction()

function(sb_set_target_common_private_properties TARGET_NAME)
    set(WARNING_IGNORE_LIST 
        /wd4068 
        /wd4996
        /wd4307
        /wd4521)

    target_compile_options(${TARGET_NAME} 
        INTERFACE 
            /Wall ${WARNING_IGNORE_LIST})
endfunction()