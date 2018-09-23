include(CMakeParseArguments)

macro (sbAddLibrary LIB_NAME LIB_TYPE)

	add_library(${LIB_NAME} ${LIB_TYPE} ${ARGN})
	add_library(sb::${LIB_NAME} ALIAS ${LIB_NAME})

	target_link_libraries(${LIB_NAME} 
	 	PRIVATE
	 		sb::common_private 
		PUBLIC
			sb::common_public)

endmacro ()

macro (sbAddLibraryTest LIB_NAME)

    if (SB_GENERATE_UNIT_TESTS)

        set(target_name ${LIB_NAME}_test)

        add_executable(${target_name} ${ARGN})

        target_link_libraries(${target_name} 
            PRIVATE
                sb::common_private sb::common_public sb::${LIB_NAME} gtest_ext)

    endif()

endmacro ()

macro(sbAddExecutable EXE_NAME)

	add_executable(${EXE_NAME} ${ARGN})	

	target_link_libraries(${EXE_NAME}
		PRIVATE
			sb::common_private sb::common_public)

endmacro()
