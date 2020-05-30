include(make/sb_toolchain_${SB_TARGET_PLATFORM_ID}_${SB_TOOLCHAIN_ID}.cmake)

function(sb_set_target_properties TARGET_NAME)

    target_include_directories(${TARGET_NAME} INTERFACE 
        ${SB_ENGINE_SRC_DIR_PATH} 
        ${SB_EXTERN_INC_DIR_PATH}
        ${SB_EXTERN_INCX_DIR_PATH}) 

    _sb_set_target_properties(${TARGET_NAME})

endfunction()