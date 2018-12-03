include(TestBigEndian)

## Supported Target platforms
set(SB_TARGET_LINUX FALSE)
set(SB_TARGET_MACOS FALSE)

## Supported Target toolchains
set(SB_TOOLCHAIN_CLANG FALSE)

## Common Engine CMake targets
add_library(common_private INTERFACE)
add_library(sb::common_private ALIAS common_private)

add_library(common_public INTERFACE)
target_include_directories(common_public
    INTERFACE
        ${SB_SRC_3RD_EXT_DIR_PATH} ${SB_SRC_3RD_INCLUDE_DIR_PATH} ${SB_SRC_ENGINE_DIR_PATH})
add_library(sb::common_public ALIAS common_public)

## Target identification
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i386")
	set(SB_TARGET_CPU "x86")
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	   set(SB_TARGET_OS_ARCH "64")
       target_compile_definitions(common_public
            INTERFACE 
                SB_TARGET_64_BIT)
	else()
	    message(FATAL_ERROR "Unsupported operating system architecture: ${CMAKE_SIZEOF_VOID_P} bits")
	endif()
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    set(SB_TARGET_CPU "x86")
    set(SB_TARGET_OS_ARCH "64")
    target_compile_definitions(common_public
        INTERFACE 
            SB_TARGET_64_BIT SB_TARGET_X86)
else()
    message(FATAL_ERROR "Unsupported processor architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

TEST_BIG_ENDIAN(is_big_endian)
if(is_big_endian)
    message(FATAL_ERROR "Big endian architecture is not supported")
else()
    target_compile_definitions(common_public
        INTERFACE 
            SB_TARGET_LITTLE_ENDIAN)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(SB_TARGET_OS "macos")
    set(SB_TARGET_MACOS TRUE)
    target_compile_definitions(common_public
        INTERFACE 
            SB_TARGET_MACOS)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(SB_TARGET_OS "linux")	
    set(SB_TARGET_LINUX TRUE)
    target_compile_definitions(common_public
        INTERFACE 
            SB_TARGET_LINUX)
else()
    message(FATAL_ERROR "Unknown target: ${CMAKE_SYSTEM_NAME}")
endif()

set(SB_TARGET_FULL_NAME ${SB_TARGET_OS}_${SB_TARGET_CPU}_${SB_TARGET_OS_ARCH})
set(SB_3RD_LIB_DIR_PATH "${SB_SRC_3RD_BASE_LIB_DIR_PATH}/${SB_TARGET_FULL_NAME}/release")
set(SB_3RD_BIN_DIR_PATH "${SB_SRC_3RD_BASE_BIN_DIR_PATH}/${SB_TARGET_FULL_NAME}/release")

## Target toolchain options
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(SB_TOOLCHAIN_CLANG TRUE)
    set(SB_TOOLCHAIN_NAME "clang")
    set(SB_TOOLCHAIN_CLANG_WARNING_IGNORE_LIST  
        "-Wno-c++98-compat" 
        "-Wno-c++98-compat-pedantic"
        "-Wno-gnu-zero-variadic-macro-arguments" 
        "-Wno-exit-time-destructors"
        "-Wno-documentation"
        "-Wno-global-constructors"
        "-Wno-weak-vtables"
        "-Wno-extra-semi"
        "-Wno-padded"
        "-Wno-missing-variable-declarations"
        "-Wno-gnu-anonymous-struct"
        "-Wno-nested-anon-types"
        "-Wno-undef"
        "-Wno-old-style-cast"
        "-Wno-implicit-fallthrough" 
        "-Wno-missing-prototypes"
        "-Wno-documentation-unknown-command"
        "-Wno-undefined-func-template")
    target_compile_definitions(common_public
        INTERFACE 
            SB_COMPILER_CLANG)
    target_link_libraries(common_public
        INTERFACE
            -lstdc++)
    target_compile_options(common_private
        INTERFACE 
            -std=c++2a -g -fno-exceptions -Wall -Weverything -Werror ${SB_TOOLCHAIN_CLANG_WARNING_IGNORE_LIST} )
else ()
    message(FATAL_ERROR "Unknown toolchain: ${CMAKE_CXX_COMPILER_ID}")
endif ()

