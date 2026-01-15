cmake_minimum_required(VERSION 4.0)

project(rx
    VERSION 0.0.1
    LANGUAGES CXX
)

add_library(rx INTERFACE)
target_include_directories(rx INTERFACE include/)
target_compile_features(rx INTERFACE cxx_std_26)

add_compile_options(-Wall -Wextra)

add_library(tests OBJECT)
target_sources(tests
    PRIVATE
        tests/bset_test.cpp
        tests/cset_test.cpp
        tests/expo_test.cpp
        tests/iter_test.cpp
        tests/tdfa_test.cpp
        tests/tnfa_test.cpp
        tests/tree_test.cpp
)
target_link_libraries(tests PUBLIC rx)

add_executable(main)
target_sources(main
    PRIVATE 
        tests/_testfile.cpp
)
target_link_libraries(main PUBLIC rx)

add_subdirectory(bench/ EXCLUDE_FROM_ALL)
