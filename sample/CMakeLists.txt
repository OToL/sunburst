if(SB_ENABLE_SANPLE)
    file(GLOB children RELATIVE ${CMAKE_CURRENT_LIST_DIR} *)

    foreach(child_iter ${children})
        if(IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${child_iter})
            add_subdirectory(${child_iter})
        endif()
    endforeach()
endif()