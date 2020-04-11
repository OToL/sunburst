cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

project(sunburst VERSION 1.0.0 LANGUAGES C CXX)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
    set(SB_IS_CMAKE_SOURCE_DIR TRUE)
else()
    set(SB_IS_CMAKE_SOURCE_DIR FALSE)
endif()

# Global options
OPTION(SB_GENERATE_UNIT_TESTS "Enable unit tests executable projects" ON)

include(make/sb_utility.cmake)

include(make/sb_host_environment.cmake)
sb_setup_host_environment()

include(make/sb_target_platform.cmake)
sb_setup_target_platform()

add_library(cxx_common_private INTERFACE)
add_library(sb::cxx_common_private ALIAS cxx_common_private)

add_library(cxx_common_public INTERFACE)
add_library(sb::cxx_common_public ALIAS cxx_common_public)
target_compile_features(cxx_common_public INTERFACE cxx_std_17)
target_include_directories(cxx_common_public INTERFACE ${SB_SRC_ENGINE_DIR_PATH} ${SB_SRC_3RD_INCLUDE_DIR_PATH})

string(TOLOWER ${CMAKE_CXX_COMPILER_ID} SB_TOOLCHAIN_ID)

include(make/sb_toolchain_${SB_TARGET_PLATFORM_ID}_${SB_TOOLCHAIN_ID}.cmake)
sb_set_target_common_public_properties(cxx_common_public)
sb_set_target_common_private_properties(cxx_common_private)

add_subdirectory("src/engine")
# add_subdirectory("src/test")
# add_subdirectory("src/thirdparty")