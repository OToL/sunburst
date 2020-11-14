function(sb_setup_platform)
    
    if(((${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i386") AND (CMAKE_SIZEOF_VOID_P EQUAL 8)) OR
        (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64") OR 
        (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64"))
        set(TARGET_CPU_ARCH_ID "64")
        set(TARGET_CPU_FAMILY_NAME "x86")
        set(TARGET_CPU_ID "x86_64")
        set(TARGET_CPU_ARCH_64B TRUE)
    else()
        message(FATAL_ERROR "Unsupported CPU: ${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_SIZEOF_VOID_P}")
    endif()

    if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        set(TARGET_PLATFORM_ID "macos")
        set(TARGET_PLATFORM_FAMILY_ID "unix")
        set(TARGET_PLATFORM_FILE_SUFFIX "macos") 
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        set(TARGET_PLATFORM_ID "linux")	
        set(TARGET_PLATFORM_FAMILY_ID "unix")
        set(TARGET_PLATFORM_FILE_SUFFIX "linux") 
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set(TARGET_PLATFORM_ID "windows")	
        set(TARGET_PLATFORM_FAMILY_ID "windows")
        set(TARGET_PLATFORM_FILE_SUFFIX "win") 
    else()
        message(FATAL_ERROR "Unknown target: ${CMAKE_SYSTEM_NAME}")
    endif()

    set(TARGET_PLATFORM_FULL_ID ${TARGET_PLATFORM_ID}_${TARGET_CPU_ID})

    set(SB_TARGET_CPU_ARCH_ID "${TARGET_CPU_ARCH_ID}" PARENT_SCOPE)
    set(SB_TARGET_CPU_FAMILY_NAME "${TARGET_CPU_FAMILY_NAME}" PARENT_SCOPE)
    set(SB_TARGET_CPU_ID "${TARGET_CPU_ID}" PARENT_SCOPE)
    set(SB_TARGET_CPU_ARCH_64B "${TARGET_CPU_ARCH_64B}" PARENT_SCOPE)
    set(SB_TARGET_PLATFORM_FULL_ID "${TARGET_PLATFORM_FULL_ID}" PARENT_SCOPE)
    set(SB_EXTERN_LIB_DIR_PATH "${SB_EXTERN_LIB_DIR_PATH}/${TARGET_PLATFORM_FULL_ID}" PARENT_SCOPE)
    set(SB_EXTERN_BIN_DIR_PATH "${SB_EXTERN_BIN_DIR_PATH}/${TARGET_PLATFORM_FULL_ID}" PARENT_SCOPE)
    set(SB_TARGET_PLATFORM_ID "${TARGET_PLATFORM_ID}" PARENT_SCOPE)	
    set(SB_TARGET_PLATFORM_FAMILY_ID "${TARGET_PLATFORM_FAMILY_ID}" PARENT_SCOPE)
    set(SB_TARGET_PLATFORM_FILE_SUFFIX "${TARGET_PLATFORM_FILE_SUFFIX}" PARENT_SCOPE)

endfunction()