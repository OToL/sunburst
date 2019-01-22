
if(SB_TARGET_LINUX)
    add_library(gtest STATIC IMPORTED)
    set_target_properties(gtest PROPERTIES
        INTERFACE_COMPILE_DEFINITIONS "GTEST_USE_OWN_TR1_TUPLE=1;GTEST_HAS_PTHREAD=0"
        INTERFACE_LINK_LIBRARIES "-lpthread"
        IMPORTED_LOCATION ${SB_3RD_LIB_DIR_PATH}/libgtest.a)    
        
    add_library(vulkan SHARED IMPORTED)
    set_target_properties(vulkan PROPERTIES
        IMPORTED_LOCATION ${SB_3RD_BIN_DIR_PATH}/libvulkan.so)

    add_library(glfw STATIC IMPORTED)
    set_target_properties(glfw PROPERTIES
        INTERFACE_LINK_LIBRARIES "-lpthread;-ldl;-lX11"
        IMPORTED_LOCATION ${SB_3RD_LIB_DIR_PATH}/libglfw3.a)
elseif(SB_TARGET_MACOS)
    add_library(gtest STATIC IMPORTED)
    set_target_properties(gtest PROPERTIES
        INTERFACE_COMPILE_DEFINITIONS "GTEST_USE_OWN_TR1_TUPLE=1;GTEST_HAS_PTHREAD=0"
        INTERFACE_LINK_LIBRARIES "-lpthread"
        IMPORTED_LOCATION ${SB_3RD_LIB_DIR_PATH}/libgtest.a)   
elseif(SB_TARGET_WINDOWS)
    add_library(gtest STATIC IMPORTED)
    set_target_properties(gtest PROPERTIES
        IMPORTED_LOCATION ${SB_3RD_LIB_DIR_PATH}/gtest.lib)    
else()
    message(FATAL_ERROR "Thirdparty libraries not configured for ${SB_TARGET_OS}")
endif()
