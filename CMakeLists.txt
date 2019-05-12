cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

project(sunburst LANGUAGES CXX)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
    set(SB_IS_CMAKE_SOURCE_DIR TRUE)
else()
    set(SB_IS_CMAKE_SOURCE_DIR FALSE)
endif()

# Global options
OPTION(SB_GENERATE_UNIT_TESTS "Enable unit tests executable projects" ON)

# Common host environment paths
set(SB_ROOT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(SB_TEST_DIR_NAME "_test")
set(SB_SRC_ENGINE_DIR_PATH ${SB_ROOT_DIR_PATH}/src/engine)
set(SB_SRC_3RD_INCLUDE_DIR_PATH ${SB_ROOT_DIR_PATH}/src/thirdparty/inc)
set(SB_SRC_3RD_EXT_DIR_PATH ${SB_ROOT_DIR_PATH}/src/thirdparty/ext)
set(SB_SRC_3RD_LIB_DIR_PATH "${SB_ROOT_DIR_PATH}/src/thirdparty/lib")
set(SB_SRC_3RD_BIN_DIR_PATH "${SB_ROOT_DIR_PATH}/src/thirdparty/bin")

## Mandatory tools
find_package(PythonInterp 3 REQUIRED)
set(SB_PYTHON_EXECUTABLE ${PYTHON_EXECUTABLE})

if(SB_IS_CMAKE_SOURCE_DIR)
    include(make/sb_utility.cmake)
    include(make/sb_target.cmake)
    include(make/sb_thirdparty.cmake)
endif()

add_subdirectory("src/engine")
add_subdirectory("src/thirdparty")
