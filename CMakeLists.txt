cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

project(sunburst VERSION 1.0.0 LANGUAGES C CXX)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
    set(SB_IS_CMAKE_SOURCE_DIR TRUE)
else()
    set(SB_IS_CMAKE_SOURCE_DIR FALSE)
endif()

# Global options
OPTION(SB_ENABLE_TESTS "Enable unit tests executable targets" ON)
OPTION(SB_WARNINGS_AS_ERRORS "Treat compilation warnings as errors" ON)

# Custom cmake helpers
include(make/sb_utility.cmake)

# Utility target providing common settings such as c++ standard version, inclusion paths, compilation warnings, etc. 
add_library(sb_cxx_properties_public INTERFACE)
add_library(sb_cxx_properties_private INTERFACE)
add_library(sb::cxx_properties_public ALIAS sb_cxx_properties_public)
add_library(sb::cxx_properties_private ALIAS sb_cxx_properties_private)

include(make/sb_environment.cmake)
include(make/sb_platform.cmake)

sb_setup_environment(${CMAKE_CURRENT_LIST_DIR})
sb_setup_platform()

include(make/sb_toolchain.cmake)

sb_setup_toolchain()
sb_set_target_properties(sb_cxx_properties)
sb_set_target_warnings(sb_cxx_properties)

include(make/sb_extern.cmake)
sb_setup_extern()

add_subdirectory("engine")
add_subdirectory("sample")

if(SB_ENABLE_TESTS)
    enable_testing()
    add_subdirectory("test")
endif()