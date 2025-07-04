cmake_minimum_required(VERSION 4.0.3 FATAL_ERROR)

# set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

project(rx
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 26)
# set(CMAKE_CXX_MODULE_STD 1)


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


# #
# # Import the modules from libc++
# #

# include(FetchContent)
# FetchContent_Declare(
#   std
#   URL "file://${LIBCXX_BUILD}/modules/c++/v1/"
#   DOWNLOAD_EXTRACT_TIMESTAMP TRUE
#   SYSTEM
# )
# FetchContent_MakeAvailable(std)


# ## END



add_library(rx)
add_library("rx.util")
add_library("rx.ast")
add_library("rx.fsm")
add_library("rx.tests")


target_sources("rx.util"
    PUBLIC
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        BASE_DIRS src/rx/util
        FILES
            src/rx/util/cdarray.cpp
            src/rx/util/error.cpp
            src/rx/util/partition.cpp
            src/rx/util/util.cpp
)

# target_link_libraries("rx.util" PRIVATE std)

target_sources("rx.ast"
    PUBLIC
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        BASE_DIRS src/rx/ast
        FILES
            src/rx/ast/captures.cpp
            src/rx/ast/charclass.cpp
            src/rx/ast/tok.cpp
            src/rx/ast/tree.cpp
)

target_link_libraries("rx.ast" PRIVATE "rx.util")
# target_link_libraries("rx.ast" PRIVATE "rx.util" std)


target_sources("rx.fsm"
    PUBLIC
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        BASE_DIRS src/rx/fsm
        FILES
            src/rx/fsm/fsm.cpp
            src/rx/fsm/tdfa.cpp
            src/rx/fsm/tnfa.cpp
)

target_link_libraries("rx.fsm" PRIVATE "rx.util" "rx.ast")
# target_link_libraries("rx.fsm" PRIVATE "rx.util" "rx.ast" std)


target_sources(rx
    PUBLIC
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        BASE_DIRS src/rx
        FILES
            src/rx/rx.cpp
)

target_link_libraries(rx PRIVATE "rx.util" "rx.ast" "rx.fsm")
# target_link_libraries(rx PRIVATE "rx.util" "rx.ast" "rx.fsm" std)

target_sources("rx.tests"
    PUBLIC
        FILE_SET cxx_modules
        TYPE CXX_MODULES
        BASE_DIRS tests/util
        FILES
            tests/util/all.cpp
            tests/util/printable_tree.cpp
            tests/util/tnfa_matcher.cpp
            tests/util/tree_matcher.cpp
)

target_link_libraries("rx.tests" PRIVATE rx "rx.util" "rx.ast" "rx.fsm")
# target_link_libraries("rx.tests" PRIVATE rx "rx.util" "rx.ast" "rx.fsm" std)

add_library(tests)
target_sources(tests
    PUBLIC
        tests/expo_test.cpp
        tests/part_test.cpp
        tests/tnfa_test.cpp
        tests/tree_test.cpp
)

target_link_libraries(tests PRIVATE rx "rx.util" "rx.ast" "rx.fsm" "rx.tests")
# target_link_libraries(tests PRIVATE rx "rx.util" "rx.ast" "rx.fsm" "rx.tests" std)

add_executable(main)
target_sources(main
    PRIVATE 
        testfile.cpp
)

target_link_libraries(main PRIVATE rx "rx.tests")
# target_link_libraries(main PRIVATE rx "rx.tests" std)


