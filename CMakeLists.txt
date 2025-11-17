cmake_minimum_required(VERSION 4.0)


project(rx
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 26)


add_compile_options(-Wall -Wextra)
add_compile_options(-freflection-latest)

add_compile_options(
    -nostdinc++
    -isystem ${LIBCXX_BUILD}/include/c++/v1
    -I ${LIBCXX_BUILD}/include/x86_64-unknown-linux-gnu/c++/v1/
)

add_link_options(
    -nostdlib++
    -L ${LIBCXX_BUILD}/lib
    -lc++
    -Wl,--rpath=${LIBCXX_BUILD}/lib/x86_64-unknown-linux-gnu
)

include_directories(include/)

set(CMAKE_LINKER_TYPE LLD)

add_library(tests)
target_sources(tests
    PUBLIC
        tests/bset_test.cpp
        tests/cset_test.cpp
        tests/expo_test.cpp
        tests/part_test.cpp
        tests/tdfa_test.cpp
        tests/tnfa_test.cpp
        tests/tree_test.cpp
)

add_executable(main)
target_sources(main
    PRIVATE 
        tests/_testfile.cpp
)


