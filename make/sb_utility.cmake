include(CMakeParseArguments)

# http://stackoverflow.com/questions/7172670/best-shortest-way-to-join-a-list-in-cmake
function(sbJoinList GLUE VALUES _RES)

	set(tmp_result "")
    set(tmp_glue "")
 
    foreach(arg ${VALUES})
        set(tmp_result "${tmp_result}${tmp_glue}${arg}")
        set(tmp_glue "${GLUE}")
    endforeach()

    set(${_RES} "${tmp_result}" PARENT_SCOPE)

endfunction()

function(sbExecPythonScript SCRIPT_PATH SCRIPT_ARGS WORKING_DIR)

    set(one_value_args RES STDOUT)
    CMAKE_PARSE_ARGUMENTS(WORKING_DIR "" "${one_value_args}" "" ${ARGN} )

    set(rv 0)
    execute_process(COMMAND ${SB_PYTHON_EXECUTABLE} ${SCRIPT_PATH} ${SCRIPT_ARGS} RESULT_VARIABLE rv WORKING_DIRECTORY ${WORKING_DIR} OUTPUT_VARIABLE stdout ERROR_VARIABLE stderr)
 
	if (NOT ${rv} EQUAL 0)
	    message("fail to execute: ${SCRIPT_PATH} ${SCRIPT_ARGS}")
		message("return value: ${rv}")
		message("standard output: ${stdout}")
		message("standard error: ${stderr}")
	endif()

    if (DEFINED WORKING_DIR_STDOUT)
        set(${WORKING_DIR_STDOUT} ${stdout} PARENT_SCOPE)
    endif()
    
    if (DEFINED WORKING_DIR_RES)
        set(${WORKING_DIR_RES} ${rv} PARENT_SCOPE)
    endif()

endfunction()
