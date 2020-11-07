include(make/sb_toolchain_${SB_TARGET_PLATFORM_ID}_${SB_TOOLCHAIN_ID}.cmake)

function(sb_setup_toolchain_properties BASE_TARGET_NAME)

    target_include_directories(${BASE_TARGET_NAME}_public INTERFACE 
        ${SB_ENGINE_SRC_DIR_PATH} 
        ${SB_EXTERN_INC_DIR_PATH}
        ${SB_EXTERN_INCX_DIR_PATH}) 

    _sb_setup_toolchain_properties(${BASE_TARGET_NAME})

endfunction()
