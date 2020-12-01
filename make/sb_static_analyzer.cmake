
if(SB_ENABLE_CLANG_TIDY)

    if(SB_TOOLCHAIN_ID STREQUAL "clang")
        set(CLANG_TIDY_PATH "${SB_EXTERN_APP_DIR_PATH}/clang-tidy${SB_APP_EXTENSION}")
        if(EXISTS ${CLANG_TIDY_PATH})
            set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_PATH}")
        else()
            message(FATAL_ERROR "Unable to find clang-tidy")
        endif()
    else()
        message(STATUS "Clang-tidy is not available with toolchain '${SB_TOOLCHAIN_ID}'")
    endif()

endif()

