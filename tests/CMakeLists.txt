project(rx-tests)

add_library(tests OBJECT)

set_target_properties(tests PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_options(tests PRIVATE -Wall -Wextra -Wnrvo -Wno-maybe-musttail-local-addr)

target_sources(tests
    PRIVATE
        expo_test.cpp
        iter_test.cpp
        lex1_test.cpp
        lex2_test.cpp
        repl_test.cpp
        setb_test.cpp
        setc_test.cpp
        srx1_test.cpp
        srx2_test.cpp
        srxc_test.cpp
        srxf_test.cpp
        srxn_test.cpp
        tdfa_test.cpp
        tdfb_test.cpp
        tnfa_test.cpp
)

target_link_libraries(tests PUBLIC static_regex)