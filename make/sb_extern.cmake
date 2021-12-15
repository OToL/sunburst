set(CONANA_FILE_LIST "conanbuildinfo.cmake" "../conanbuildinfo.cmake" "conanbuildinfo_multi.cmake" "../conanbuildinfo_multi.cmake")
foreach(CONAN_FILE ${CONANA_FILE_LIST})
    set(CONAN_FILE_PATH "${CMAKE_BINARY_DIR}/${CONAN_FILE}")
    message(STATUS "Conan: checking ${CONAN_FILE_PATH}")
    if(EXISTS "${CONAN_FILE_PATH}")
		set(CONAN_CMAKE_FILE_PATH "${CONAN_FILE_PATH}")
        if(CONAN_FILE MATCHES "_multi.cmake")
            set(CONAN_CMAKE_MULTI TRUE)
        endif()
        break()
    endif()
endforeach()

if(NOT CONAN_CMAKE_FILE_PATH)
    message(FATAL_ERROR "Cannot locate conanbuildinfo.cmake, did you run make/gen_conan?")
endif()

include(${CONAN_CMAKE_FILE_PATH})

function(sb_alias_conan_target CONAN_TARGET SB_TARGET)

    add_library(sb_extern_${SB_TARGET} INTERFACE)
    add_library(sbext::${SB_TARGET} ALIAS sb_extern_${SB_TARGET})
    target_link_libraries(sb_extern_${SB_TARGET}
        INTERFACE
            CONAN_PKG::${CONAN_TARGET})

endfunction()

function(sb_setup_extern)

    conan_basic_setup(TARGETS)

    sb_alias_conan_target(catch2 catch)
    if(CONAN_CMAKE_MULTI)
        set(SB_CATCH_CMAKE_DIR_PATH "${CONAN_CATCH2_ROOT_RELEASE}/lib/cmake/Catch2" PARENT_SCOPE)
    else()
        set(SB_CATCH_CMAKE_DIR_PATH "${CONAN_CATCH2_ROOT}/lib/cmake/Catch2" PARENT_SCOPE)
    endif()

    sb_alias_conan_target(stb stb)
    target_compile_definitions(CONAN_PKG::stb
        INTERFACE
            STB_IMAGE_IMPLEMENTATION) 

    sb_alias_conan_target(glm glm)
    target_compile_definitions(CONAN_PKG::glm 
        INTERFACE
            GLM_FORCE_RADIANS
            GLM_FORCE_DEPTH_ZERO_TO_ONE
            GLM_FORCE_DEFAULT_ALIGNED_GENTYPES)

    if(SB_TARGET_PLATFORM_ID STREQUAL "windows")
        if(SB_TOOLCHAIN_ID STREQUAL "msvc")
            add_library(sbext::asan SHARED IMPORTED)
            set_target_properties(sbext::asan 
                PROPERTIES 
                    IMPORTED_LOCATION ${SB_EXTERN_BIN_DIR_PATH}/clang_rt.asan_dynamic-x86_64.dll
                    IMPORTED_IMPLIB ${SB_EXTERN_LIB_DIR_PATH}/stub.lib)
            target_link_options(sbext::asan
                INTERFACE
                    /wholearchive:${SB_EXTERN_LIB_DIR_PATH}/clang_rt.asan_dynamic-x86_64.lib 
                    /wholearchive:${SB_EXTERN_LIB_DIR_PATH}/clang_rt.asan_dynamic_runtime_thunk-x86_64.lib)
        else()
            message(STATUS "Sanitizers are not available with toolchain '${SB_TOOLCHAIN_ID}'")
        endif()
    endif()

endfunction()
