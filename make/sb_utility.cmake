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

# https://github.com/onqtam/ucm/blob/master/cmake/ucm.cmake
macro(sbGatherFlags with_linker result)
    set(${result} "")
    # add the main flags without a config
    list(APPEND ${result} CMAKE_C_FLAGS)
    list(APPEND ${result} CMAKE_CXX_FLAGS)
    if(${with_linker})
        list(APPEND ${result} CMAKE_EXE_LINKER_FLAGS)
        list(APPEND ${result} CMAKE_MODULE_LINKER_FLAGS)
        list(APPEND ${result} CMAKE_SHARED_LINKER_FLAGS)
        list(APPEND ${result} CMAKE_STATIC_LINKER_FLAGS)
    endif()
    
    if("${CMAKE_CONFIGURATION_TYPES}" STREQUAL "" AND NOT "${CMAKE_BUILD_TYPE}" STREQUAL "")
        # handle single config generators - like makefiles/ninja - when CMAKE_BUILD_TYPE is set
        string(TOUPPER ${CMAKE_BUILD_TYPE} config)
        list(APPEND ${result} CMAKE_C_FLAGS_${config})
        list(APPEND ${result} CMAKE_CXX_FLAGS_${config})
        if(${with_linker})
            list(APPEND ${result} CMAKE_EXE_LINKER_FLAGS_${config})
            list(APPEND ${result} CMAKE_MODULE_LINKER_FLAGS_${config})
            list(APPEND ${result} CMAKE_SHARED_LINKER_FLAGS_${config})
            list(APPEND ${result} CMAKE_STATIC_LINKER_FLAGS_${config})
        endif()
    else()
        # handle multi config generators (like msvc, xcode)
        foreach(config ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER ${config} config)
            list(APPEND ${result} CMAKE_C_FLAGS_${config})
            list(APPEND ${result} CMAKE_CXX_FLAGS_${config})
            if(${with_linker})
                list(APPEND ${result} CMAKE_EXE_LINKER_FLAGS_${config})
                list(APPEND ${result} CMAKE_MODULE_LINKER_FLAGS_${config})
                list(APPEND ${result} CMAKE_SHARED_LINKER_FLAGS_${config})
                list(APPEND ${result} CMAKE_STATIC_LINKER_FLAGS_${config})
            endif()
        endforeach()
    endif()
endmacro()

macro(sbSetRuntimeLibrary)
    cmake_parse_arguments(ARG "STATIC;DYNAMIC" "" "" ${ARGN})

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "unrecognized arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()
    
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" STREQUAL "")
        message(AUTHOR_WARNING "ucm_set_runtime() does not support clang yet!")
    endif()
    
    sbGatherFlags(0 flags_configs)
    
    # add/replace the flags
    # note that if the user has messed with the flags directly this function might fail
    # - for example if with MSVC and the user has removed the flags - here we just switch/replace them
    if("${ARG_STATIC}")
        foreach(flags ${flags_configs})
            if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
                if(NOT ${flags} MATCHES "-static-libstdc\\+\\+")
                    set(${flags} "${${flags}} -static-libstdc++")
                endif()
                if(NOT ${flags} MATCHES "-static-libgcc")
                    set(${flags} "${${flags}} -static-libgcc")
                endif()
            elseif(MSVC)
                if(${flags} MATCHES "/MD")
                    string(REGEX REPLACE "/MD" "/MT" ${flags} "${${flags}}")
                endif()
            endif()
        endforeach()
    elseif("${ARG_DYNAMIC}")
        foreach(flags ${flags_configs})
            if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
                if(${flags} MATCHES "-static-libstdc\\+\\+")
                    string(REGEX REPLACE "-static-libstdc\\+\\+" "" ${flags} "${${flags}}")
                endif()
                if(${flags} MATCHES "-static-libgcc")
                    string(REGEX REPLACE "-static-libgcc" "" ${flags} "${${flags}}")
                endif()
            elseif(MSVC)
                if(${flags} MATCHES "/MT")
                    string(REGEX REPLACE "/MT" "/MD" ${flags} "${${flags}}")
                endif()
            endif()
        endforeach()
    endif()
endmacro()
