cmake_minimum_required(VERSION 3.10)
project(julia-bind)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(TESTS "enable/disable tests" ON)

add_compile_options(
    -Wall
    -Wextra
    -Werror
    -Wshadow
    -Wnon-virtual-dtor
    -pedantic
    )

include_directories(include)

if (TESTS)
    add_subdirectory(tests)
endif ()

