include(${CMAKE_BINARY_DIR}/conan.cmake)

function(sb_alias_conan_target CONAN_TARGET SB_TARGET)

    add_library(sb_extern_${SB_TARGET} INTERFACE)
    add_library(sbext::${SB_TARGET} ALIAS sb_extern_${SB_TARGET})
    target_link_libraries(sb_extern_${SB_TARGET}
        INTERFACE
            CONAN_PKG::${CONAN_TARGET})

endfunction()

function(sb_setup_extern)
    
    conan_cmake_run(
        REQUIRES
            catch2/2.11.3
        BASIC_SETUP
            CMAKE_TARGETS) 
    conan_load_buildinfo()

    sb_alias_conan_target(catch2 catch)

    # Visual Studio supports multiple configurations at once and catch2 cmake files are separated
    if(CONAN_CMAKE_MULTI)
        set(SB_CATCH_CMAKE_DIR_PATH "${CONAN_CATCH2_ROOT_RELEASE}/lib/cmake/Catch2" PARENT_SCOPE)
    elseif()
        set(SB_CATCH_CMAKE_DIR_PATH "${CONAN_CATCH2_ROOT}/lib/cmake/Catch2" PARENT_SCOPE)
    endif()

    set(SB_CATCH_CMAKE_DIR_PATH "${CONAN_CATCH2_ROOT}/lib/cmake/Catch2" PARENT_SCOPE)

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