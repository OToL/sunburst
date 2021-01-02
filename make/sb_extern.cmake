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
            glm/0.9.9.8
            stb/20200203
            glfw/3.3.2 
            tinyobjloader/1.0.6
        BASIC_SETUP
            CMAKE_TARGETS) 
    conan_load_buildinfo()

    sb_alias_conan_target(catch2 catch)
    
    sb_alias_conan_target(tinyobjloader tinyobjloader)
    target_compile_definitions(CONAN_PKG::tinyobjloader
        INTERFACE 
            TINYOBJLOADER_IMPLEMENTATION)
    
    sb_alias_conan_target(glfw glfw)
    target_compile_definitions(CONAN_PKG::glfw
        INTERFACE 
            GLFW_INCLUDE_VULKAN)    
        
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

    if(SB_TOOLCHAIN_ID STREQUAL "msvc")
        set(SB_EXTERN_SOURCES_COMPILATION_WARNINGS "/wd4255 /wd5220 /wd4365 /wd5039 /wd4430 /wd4668" PARENT_SCOPE)
    elseif(SB_TOOLCHAIN_ID STREQUAL "clang")
        set(SB_EXTERN_SOURCES_COMPILATION_WARNINGS "-Wno-reserved-id-macro -Wno-static-in-inline" PARENT_SCOPE)
    endif()

endfunction()