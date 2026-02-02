project(rx-trace-compile)

set(PATTERNS
    [["Twain"]]
    [["(?i)Twain"]]
    [["[a-z]shing"]]
    [["Huck[a-zA-Z]+|Saw[a-zA-Z]+"]]
    [["\\b\\w+nn\\b"]]
    # [["[a-q][^u-z]{13}x"]]
    [["Tom|Sawyer|Huckleberry|Finn"]]
    [["(?i)Tom|Sawyer|Huckleberry|Finn"]]
    [[".{0,2}(Tom|Sawyer|Huckleberry|Finn)"]]
    [[".{2,4}(Tom|Sawyer|Huckleberry|Finn)"]]
    # [["Tom.{10,25}river|river.{10,25}Tom"]]
    [["[a-zA-Z]+ing"]]
    [["\\s[a-zA-Z]{0,12}ing\\s"]]
    [["([A-Za-z]awyer|[A-Za-z]inn)\\s"]]
    [["[\"'][^\"']{0,30}[?!\\.][\\\"']"]]
)

add_library(trace-interface INTERFACE)
target_compile_options(trace-interface INTERFACE -ftime-trace -ftime-trace-granularity=1)
set_target_properties(trace-interface PROPERTIES FOLDER compile-time-trace)

function(MAKE_TRACE PATTERN INDEX)
    string(MAKE_C_IDENTIFIER "${PATTERN}" C_PATTERN)
    set(CURRENT_NAME "trace-${INDEX}-${C_PATTERN}")
    add_library(${CURRENT_NAME} OBJECT)
    target_compile_definitions(${CURRENT_NAME} PUBLIC REGEX_PATTERN="$<QUOTE>${PATTERN}$<QUOTE>" FUNCTION_NAME=test_${C_PATTERN})
    target_sources(${CURRENT_NAME} PRIVATE compile_stub.cpp)
    target_link_libraries(${CURRENT_NAME} PUBLIC rx trace-interface)
    set_target_properties(${CURRENT_NAME} PROPERTIES FOLDER compile-time-trace)
    add_dependencies(trace ${CURRENT_NAME})
endfunction()

add_custom_target(trace)
set(COUNTER 0)
MAKE_TRACE("" ${COUNTER})

foreach(PATTERN ${PATTERNS})
    math(EXPR COUNTER "${COUNTER}+1")
    MAKE_TRACE("${PATTERN}" "${COUNTER}")
endforeach()


find_package(benchmark REQUIRED)

add_executable(profile-target)
target_sources(profile-target PRIVATE profile_target.cpp)
target_link_libraries(profile-target PUBLIC rx benchmark::benchmark)
target_compile_options(profile-target PUBLIC -fno-omit-frame-pointer)