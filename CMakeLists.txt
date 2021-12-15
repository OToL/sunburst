cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

project(sunburst VERSION 1.0.0 LANGUAGES C CXX)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
    set(SB_IS_CMAKE_SOURCE_DIR TRUE)
else()
    set(SB_IS_CMAKE_SOURCE_DIR FALSE)
endif()

# Global options
OPTION(SB_ENABLE_UNITTEST "Enable unit tests executable targets" ON)
OPTION(SB_ENABLE_WARNING_AS_ERROR "Treat compilation warnings as errors" ON)
OPTION(SB_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
OPTION(SB_ENABLE_SANPLE "Enable sample applications" ON)
OPTION(SB_ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "Enable undefined behavior sanitizer" OFF)
OPTION(SB_ENABLE_CLANG_TIDY "Enable clang tidy static analyzer" OFF)

# Custom cmake helpers
include(make/sb_utility.cmake)

# Utility targets providing common settings such as c++ standard version, inclusion paths, compilation warnings, etc. 
add_library(sb_cxx_common_public INTERFACE)
add_library(sb_cxx_common_private INTERFACE)
add_library(sb::cxx_common_public ALIAS sb_cxx_common_public)
add_library(sb::cxx_common_private ALIAS sb_cxx_common_private)

include(make/sb_environment.cmake)
sb_setup_environment(${CMAKE_CURRENT_LIST_DIR})

include(make/sb_platform.cmake)
sb_setup_platform()

include(make/sb_static_analyzer.cmake)

include(make/sb_extern.cmake)
sb_setup_extern()

include(make/sb_toolchain.cmake)
sb_setup_toolchain()
sb_setup_toolchain_warnings(sb_cxx_common)
sb_setup_toolchain_sanitizers(sb_cxx_common)
sb_setup_toolchain_properties(sb_cxx_common)

add_subdirectory("engine")
add_subdirectory("sample")

if(SB_ENABLE_UNITTEST)
    enable_testing()
    add_subdirectory("test")
endif()
