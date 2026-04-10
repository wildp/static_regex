project(rx-tools)

add_executable(amalgamate)
target_sources(amalgamate PRIVATE amalgamate.cpp)
target_link_libraries(amalgamate PUBLIC rx)