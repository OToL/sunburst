## Local tools detection
find_package(PythonInterp 3 REQUIRED)
set(SB_PYTHON_EXECUTABLE ${PYTHON_EXECUTABLE})

## Constants
set(SB_ROOT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
set(SB_TEST_DIR_NAME "_test")

## Target independent paths
set(SB_SRC_ENGINE_DIR_PATH ${SB_ROOT_DIR_PATH}/src/engine)
set(SB_SRC_3RD_INCLUDE_DIR_PATH ${SB_ROOT_DIR_PATH}/src/thirdparty/inc)
set(SB_SRC_3RD_EXT_DIR_PATH ${SB_ROOT_DIR_PATH}/src/thirdparty/ext)
set(SB_SRC_3RD_LIB_DIR_PATH "${SB_ROOT_DIR_PATH}/src/thirdparty/lib")
set(SB_SRC_3RD_BIN_DIR_PATH "${SB_ROOT_DIR_PATH}/src/thirdparty/bin")
