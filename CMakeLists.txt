cmake_minimum_required(VERSION 4.0)

project(rx
    VERSION 0.0.2
    LANGUAGES CXX
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(rx INTERFACE)
target_compile_features(rx INTERFACE cxx_std_26)
target_sources(rx
    INTERFACE
    FILE_SET
        HEADERS
    BASE_DIRS
        include/
    FILES
        include/rx/regex.hpp
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(rx
        INTERFACE
            -freflection-latest
            -fconstexpr-steps=4294967295
            -fconstexpr-backtrace-limit=0
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(rx
        INTERFACE
            -freflection
            -fconstexpr-ops-limit=4294967296
    )
endif()

add_subdirectory(tests/)
add_subdirectory(bench/ EXCLUDE_FROM_ALL)
add_subdirectory(trace/ EXCLUDE_FROM_ALL)
add_subdirectory(tools/ EXCLUDE_FROM_ALL)

add_custom_command(
    OUTPUT
        single-include/rx/regex.hpp
    COMMAND
       amalgamate include/ include/rx/regex.hpp single-include/rx/regex.hpp LICENSE
    WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}
)

add_custom_target(single-header
    DEPENDS
        single-include/rx/regex.hpp
)