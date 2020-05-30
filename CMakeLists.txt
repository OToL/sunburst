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
add_library(cxx_properties INTERFACE)
add_library(sb::cxx_properties ALIAS cxx_properties)

include(make/sb_environment.cmake)

sb_setup_host_environment(${CMAKE_CURRENT_LIST_DIR})
sb_setup_target_platform()

include(make/sb_toolchain.cmake)

sb_set_target_properties(cxx_properties)
sb_set_target_warnings(cxx_properties)

include(make/sb_extern.cmake)
sb_setup_extern()

add_subdirectory("engine")

if(SB_ENABLE_TESTS)
    enable_testing()
    add_subdirectory("test")
endif()