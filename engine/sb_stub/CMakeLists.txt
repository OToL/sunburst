add_library(stub STATIC
    _impl/stub.cpp)
 
add_library(sb::stub ALIAS stub)

target_link_libraries(stub 
    PRIVATE
        sb::cxx_common_private
    PUBLIC
        sb::cxx_common_public)
