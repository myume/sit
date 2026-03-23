add_executable(benchmark
  blur_benchmarks.cpp
)

target_compile_definitions(benchmark PRIVATE
    TEST_FIXTURES_DIR="${CMAKE_SOURCE_DIR}/benchmarks/fixtures"
)

target_link_libraries(benchmark PRIVATE libsit)
target_link_libraries(benchmark PRIVATE Catch2::Catch2WithMain)

target_compile_options(libsit PRIVATE -O3)
target_compile_options(benchmark PRIVATE -O3)

add_test(NAME benchmark COMMAND benchmark)
