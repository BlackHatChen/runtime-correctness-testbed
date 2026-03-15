add_executable(pool_bench pool_bench.cpp)

target_compile_features(pool_bench PRIVATE cxx_std_17)

target_include_directories(pool_bench PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(pool_bench
    PRIVATE
    rct_project_warnings
    benchmark::benchmark_main   # Google Benchmark entry point.
)