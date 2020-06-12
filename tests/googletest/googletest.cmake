# the following code to fetch googletest is inspired by and adapted after:
# https://crascit.com/2015/07/25/cmake-gtest/
# https://github.com/bast/gtest-demo
# https://github.com/abseil/googletest/blob/master/googletest/README.md

macro(fetch_googletest _googletest_root)
    set(GOOGLETEST_ROOT ${_googletest_root})
    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest-download.cmake
            ${_googletest_root}/CMakeLists.txt
            @ONLY
    )
    unset(GOOGLETEST_ROOT)

    execute_process(
            COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
            WORKING_DIRECTORY ${_googletest_root}
    )
    execute_process(
            COMMAND "${CMAKE_COMMAND}" --build .
            WORKING_DIRECTORY ${_googletest_root}
    )

    add_subdirectory(
            ${_googletest_root}/googletest-src
            ${_googletest_root}/googletest-build
    )
endmacro()
