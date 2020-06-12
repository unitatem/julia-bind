enable_testing()

include(googletest/googletest.cmake)
fetch_googletest(${PROJECT_BINARY_DIR}/googletest)

add_executable(test_built_in_types
    test_built_in_types.cpp)
target_link_libraries(test_built_in_types
    gtest
    gmock_main)
add_test(test_built_in_types test_built_in_types)
