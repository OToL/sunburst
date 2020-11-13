include(CMakeParseArguments)

function(sb_execute_python_script SCRIPT_PATH SCRIPT_ARGS WORKING_DIR)
    set(ONE_VALUE_ARGS RES STDOUT)
    cmake_parse_arguments(WORKING_DIR "" "${ONE_VALUE_ARGS}" "" ${ARGN} )

    execute_process(COMMAND ${SB_PYTHON_EXECUTABLE} ${SCRIPT_PATH} ${SCRIPT_ARGS} 
        RESULT_VARIABLE RV 
        WORKING_DIRECTORY ${WORKING_DIR}
        OUTPUT_VARIABLE STDOUT 
        ERROR_VARIABLE STDERR)
 
	if (NOT ${RV} EQUAL 0)
        message("Fail to execute: ${SCRIPT_PATH} ${SCR
            IPT_ARGS}")
		message("Return value: ${RV}")
		message("Standard output: ${STDOUT}")
		message("Standard error: ${STDERR}")
	endif()

    if (defined WORKING_DIR_STDOUT)
        set(${WORKING_DIR_STDOUT} ${STDOUT} PARENT_SCOPE)
    endif()
    
    if (defined WORKING_DIR_RES)
        set(${WORKING_DIR_RES} ${RV} PARENT_SCOPE)
    endif()
endfunction()


function(sb_get_target_link_targets TARGET_NAME TARGET_TYPES _RES)
    set(RES_LIST)
    get_target_property(LINK_LIBRARIES ${TARGET_NAME} INTERFACE_LINK_LIBRARIES)

    foreach(LIBRARY_ITER ${LINK_LIBRARIES})
        if(TARGET ${LIBRARY_ITER})
            get_target_property(TARGET_TYPE ${LIBRARY_ITER} TYPE)

            if(${TARGET_TYPE} STREQUAL TARGET_TYPES)
                list(APPEND RES_LIST ${LIBRARY_ITER})
            endif()

            sb_get_target_link_targets(${LIBRARY_ITER} "${TARGET_TYPES}" TMP_RES)

            if(TMP_RES)
                list(APPEND RES_LIST "${TMP_RES}")
            endif()
        endif()
    endforeach()

    set(${_RES} "${RES_LIST}" PARENT_SCOPE)
endfunction()

function(sb_copy_target_shared_dependencies TARGET_NAME)
    sb_get_target_link_targets(${TARGET_NAME} "SHARED_LIBRARY" LINK_TARGETS)
    list(REMOVE_DUPLICATES LINK_TARGETS)

    foreach(TARGET_ITER ${LINK_TARGETS})    
        get_target_property(TARGET_IMPORT_LOCATION ${TARGET_ITER} IMPORTED_LOCATION)
        if(TARGET_IMPORT_LOCATION)
            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        $<TARGET_FILE:${TARGET_ITER}>
                        $<TARGET_FILE_DIR:${TARGET_NAME}>)        
        endif()
    endforeach()
endfunction()