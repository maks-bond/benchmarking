#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>

static void VectorPushBackInt(benchmark::State& state) {
    std::vector<int> v;
    while(state.KeepRunning()) {
        for(int n = 0; n<state.range(0); ++n) {
            v.push_back(n); 
        }
    }
}

static void VectorInsertFrontInt(benchmark::State& state) {
    std::vector<int> v;
    while(state.KeepRunning()) {
        for(int n = 0; n<state.range(0); ++n) {
            v.insert(v.begin(), n); 
        }
    }
}

BENCHMARK(VectorPushBackInt)->Range(2, 2<<12)->Unit(benchmark::kMillisecond);
BENCHMARK(VectorInsertFrontInt)->Range(2, 2<<12)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();