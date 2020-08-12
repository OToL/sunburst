include(${SB_CATCH_CMAKE_DIR_PATH}/catch.cmake) 

add_executable(base_test
    base_test.cpp
)

target_link_libraries(base_test
    PRIVATE    
        sb::cxx_properties_private
        sb::base
        sb::unit)

catch_discover_tests(
    base_test
    TEST_PREFIX
        "base_test."
    EXTRA_ARGS
        -s
        --reporter=xml
        --out=core_test.xml)
