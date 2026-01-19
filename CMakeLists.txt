cmake_minimum_required(VERSION 4.0)

project(rx
    VERSION 0.0.1
    LANGUAGES CXX
)

# (for clang-p2996) #
add_compile_options(-fconstexpr-steps=4294967295)
add_compile_options(-fconstexpr-backtrace-limit=0)

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
        tests/srx1_test.cpp
        tests/srx2_test.cpp
        tests/tdfa_test.cpp
        tests/tnfa_test.cpp
        tests/tree_test.cpp
)
target_link_libraries(tests PUBLIC rx)

add_executable(testfile)
target_sources(testfile
    PRIVATE 
        tests/_testfile.cpp
)
target_link_libraries(testfile PUBLIC rx)

add_subdirectory(bench/ EXCLUDE_FROM_ALL)
