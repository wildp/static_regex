project(rx-bench)

find_package(benchmark REQUIRED)
find_package(ctre REQUIRED)

add_executable(bench-simple)
target_sources(bench-simple PRIVATE simple.cpp)
target_compile_options(bench-simple PRIVATE -march=native)
target_link_libraries(bench-simple PRIVATE rx benchmark::benchmark)
set_target_properties(bench-simple PROPERTIES FOLDER benchmarks)

add_executable(bench-twain)
target_sources(bench-twain PRIVATE twain.cpp)
target_compile_options(bench-twain PRIVATE -Wno-c23-extensions -march=native)
target_link_libraries(bench-twain PRIVATE rx benchmark::benchmark)
set_target_properties(bench-twain PROPERTIES FOLDER benchmarks)