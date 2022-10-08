add_library(sb_stub STATIC
    _impl/stub.cpp)
 
add_library(sb::stub ALIAS sb_stub)

target_link_libraries(sb_stub 
    PRIVATE
        sb::cxx_common_private
    PUBLIC
        sb::cxx_common_public)
