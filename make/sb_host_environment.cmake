function(sb_setup_host_environment)

    find_package(PythonInterp 3 REQUIRED)

    set(SB_PYTHON_EXECUTABLE ${PYTHON_EXECUTABLE} PARENT_SCOPE)
    set(SB_ROOT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}" PARENT_SCOPE)
    set(SB_TEST_DIR_NAME "_test" PARENT_SCOPE)
    set(SB_SRC_ENGINE_DIR_PATH ${SB_ROOT_DIR_PATH}/src/engine PARENT_SCOPE)
    set(SB_SRC_3RD_INCLUDE_DIR_PATH ${SB_ROOT_DIR_PATH}/src/thirdparty/inc PARENT_SCOPE)
    set(SB_SRC_3RD_EXT_DIR_PATH ${SB_ROOT_DIR_PATH}/src/thirdparty/ext PARENT_SCOPE)
    set(SB_SRC_3RD_LIB_DIR_PATH "${SB_ROOT_DIR_PATH}/src/thirdparty/lib" PARENT_SCOPE)
    set(SB_SRC_3RD_BIN_DIR_PATH "${SB_ROOT_DIR_PATH}/src/thirdparty/bin" PARENT_SCOPE)

endfunction()
