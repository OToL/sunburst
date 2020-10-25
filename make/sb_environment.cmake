
function(sb_setup_host_environment ROOT_DIR_PATH)

    find_package(PythonInterp 3 REQUIRED)
    string(TOLOWER ${CMAKE_CXX_COMPILER_ID} NORMALIZED_TOOLCHAIN_ID)

    set(SB_PYTHON_EXECUTABLE ${PYTHON_EXECUTABLE} PARENT_SCOPE)
    set(SB_ROOT_DIR_PATH "${ROOT_DIR_PATH}" PARENT_SCOPE)
    set(SB_ENGINE_SRC_DIR_PATH "${ROOT_DIR_PATH}/engine" PARENT_SCOPE)
    set(SB_EXTERN_INC_DIR_PATH "${ROOT_DIR_PATH}/extern/inc" PARENT_SCOPE)
    set(SB_EXTERN_INCX_DIR_PATH "${ROOT_DIR_PATH}/extern/incx" PARENT_SCOPE)
    set(SB_EXTERN_LIB_DIR_PATH "${ROOT_DIR_PATH}/extern/lib" PARENT_SCOPE)
    set(SB_EXTERN_BIN_DIR_PATH "${ROOT_DIR_PATH}/extern/bin" PARENT_SCOPE)
    set(SB_TOOLCHAIN_ID "${NORMALIZED_TOOLCHAIN_ID}" PARENT_SCOPE)

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
    endif()

endfunction()

function(sb_setup_dest_environment)
    
    if(((${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i386") AND (CMAKE_SIZEOF_VOID_P EQUAL 8)) OR
        (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64") OR 
        (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64"))
        set(SB_TARGET_CPU_ARCH_ID "64" PARENT_SCOPE)
        set(SB_TARGET_CPU_FAMILY_NAME "x86" PARENT_SCOPE)
        set(SB_TARGET_CPU_ID "x86_64" PARENT_SCOPE)
        set(SB_TARGET_CPU_ARCH_64B TRUE PARENT_SCOPE)
    else()
        message(FATAL_ERROR "Unsupported CPU: ${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_SIZEOF_VOID_P}")
    endif()

    if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        set(SB_TARGET_PLATFORM_ID "macos" PARENT_SCOPE)
        set(SB_TARGET_PLATFORM_FAMILY_ID "unix" PARENT_SCOPE)
        set(SB_TARGET_PLATFORM_FILE_SUFFIX "macos" PARENT_SCOPE) 
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        set(SB_TARGET_PLATFORM_ID "linux" PARENT_SCOPE)	
        set(SB_TARGET_PLATFORM_FAMILY_ID "unix" PARENT_SCOPE)
        set(SB_TARGET_PLATFORM_FILE_SUFFIX "linux" PARENT_SCOPE) 
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set(SB_TARGET_PLATFORM_ID "windows" PARENT_SCOPE)	
        set(SB_TARGET_PLATFORM_FAMILY_ID "windows" PARENT_SCOPE)
        set(SB_TARGET_PLATFORM_FILE_SUFFIX "win" PARENT_SCOPE) 
    else()
        message(FATAL_ERROR "Unknown target: ${CMAKE_SYSTEM_NAME}")
    endif()

    set(SB_TARGET_PLATFORM_FULL_ID ${SB_TARGET_PLATFORM_ID}_${SB_TARGET_CPU_ID} PARENT_SCOPE)
    set(SB_EXTERN_LIB_DIR_PATH "${SB_SRC_EXTERN_LIB_DIR_PATH}/${SB_TARGET_PLATFORM_FULL_ID}" PARENT_SCOPE)
    set(SB_EXTERN_BIN_DIR_PATH "${SB_SRC_EXTERN_BIN_DIR_PATH}/${SB_TARGET_PLATFORM_FULL_ID}" PARENT_SCOPE)

endfunction()
