
function(sb_setup_environment ROOT_DIR_PATH)

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
