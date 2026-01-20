project(rx-tests)

add_library(tests OBJECT)
target_sources(tests
    PRIVATE
        expo_test.cpp
        iter_test.cpp
        setb_test.cpp
        setc_test.cpp
        srx1_test.cpp
        srx2_test.cpp
        tdfa_test.cpp
        tnfa_test.cpp
        tree_test.cpp
)
target_link_libraries(tests PUBLIC rx)