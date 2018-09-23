project(sunburst LANGUAGES CXX)

# Global options
OPTION(SB_GENERATE_UNIT_TESTS "Enable unit tests executable projects" ON)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
    set(SB_IS_CMAKE_SOURCE_DIR TRUE)
else ()
    set(SB_IS_CMAKE_SOURCE_DIR FALSE)
endif ()

include(make/sb_utility.cmake)
include(make/sb_environment.cmake)
include(make/sb_target.cmake)
include(make/sb_module.cmake)
include(make/sb_thirdparty.cmake)

add_subdirectory("src/engine")
add_subdirectory("src/thirdparty")
