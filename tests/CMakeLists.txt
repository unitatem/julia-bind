enable_testing()

include(googletest/googletest.cmake)
fetch_googletest(${PROJECT_BINARY_DIR}/googletest)

include_directories("$ENV{JULIA_DIR}/include/julia")
link_directories("$ENV{JULIA_DIR}/lib")

add_executable(test_built_in_types
    test_built_in_types.cpp)
target_link_libraries(test_built_in_types
    gtest
    gmock_main
    julia)
add_test(test_built_in_types test_built_in_types)
