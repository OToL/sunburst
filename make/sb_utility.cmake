include(CMakeParseArguments)

function(sb_execute_python_scripts SCRIPT_PATH SCRIPT_ARGS WORKING_DIR)
    set(ONE_VALUE_ARGS RES STDOUT)
    cmake_parse_arguments(WORKING_DIR "" "${ONE_VALUE_ARGS}" "" ${ARGN} )

    execute_process(COMMAND ${SB_PYTHON_EXECUTABLE} ${SCRIPT_PATH} ${SCRIPT_ARGS} 
        RESULT_VARIABLE RV 
        WORKING_DIRECTORY ${WORKING_DIR}
        OUTPUT_VARIABLE STDOUT 
        ERROR_VARIABLE STDERR)
 
	if (NOT ${RV} EQUAL 0)
	    message("Fail to execute: ${SCRIPT_PATH} ${SCRIPT_ARGS}")
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