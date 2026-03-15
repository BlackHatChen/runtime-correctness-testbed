#include <benchmark/benchmark.h>

static void BM_Placeholder(benchmark::State& state) {
    // Framework decides the iterations to reach stable measurement.
    for (auto _ : state) {
        int val = 0;
        benchmark::DoNotOptimize(val += 1);
    }
}

BENCHMARK(BM_Placeholder);  // Register the benchmark.

BENCHMARK_MAIN();   // Entry point.