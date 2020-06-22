#include <benchmark/benchmark.h>
#include <vector>
#include <mutex>
#include <thread>

static void NoLock(benchmark::State& state) {
    int val = 0;
    const auto f = [&val](int times) mutable {
        for(int i = 0; i<times; ++i) {
            val++;
        }
    };
    std::vector<int> v;
    while(state.KeepRunning()) {
        std::vector<std::thread> threads;
        const int times = state.range(0);
        constexpr int thread_count = 100;
        for(int n = 0; n<thread_count; ++n) {
            threads.emplace_back(f, times);
        }
        for(int n = 0; n<thread_count; ++n) {
            threads[n].join();
        }
    }
}

static void Mutex(benchmark::State& state) {
    int val = 0;
    std::mutex m;
    const auto f = [&val, &m](int times) mutable {
        for(int i = 0; i<times; ++i) {
            m.lock();
            val++;
            m.unlock();
        }
    };
    std::vector<int> v;
    while(state.KeepRunning()) {
        std::vector<std::thread> threads;
        const int times = state.range(0);
        constexpr int thread_count = 100;
        for(int n = 0; n<thread_count; ++n) {
            threads.emplace_back(f, times);
        }
        for(int n = 0; n<thread_count; ++n) {
            threads[n].join();
        }
    }
}

static void Atomic(benchmark::State& state) {
    std::atomic<int> val(0);
    const auto f = [&val](int times) mutable {
        for(int i = 0; i<times; ++i) {
            val.fetch_add(1);
        }
    };
    std::vector<int> v;
    while(state.KeepRunning()) {
        std::vector<std::thread> threads;
        const int times = state.range(0);
        constexpr int thread_count = 100;
        for(int n = 0; n<thread_count; ++n) {
            threads.emplace_back(f, times);
        }
        for(int n = 0; n<thread_count; ++n) {
            threads[n].join();
        }
    }
}

static void AtomicRelaxed(benchmark::State& state) {
    std::atomic<int> val(0);
    const auto f = [&val](int times) mutable {
        for(int i = 0; i<times; ++i) {
            val.fetch_add(1, std::memory_order_relaxed);
        }
    };
    std::vector<int> v;
    while(state.KeepRunning()) {
        std::vector<std::thread> threads;
        const int times = state.range(0);
        constexpr int thread_count = 100;
        for(int n = 0; n<thread_count; ++n) {
            threads.emplace_back(f, times);
        }
        for(int n = 0; n<thread_count; ++n) {
            threads[n].join();
        }
    }
}

BENCHMARK(NoLock)->Range(1<<10, 1<<14)->Unit(benchmark::kMillisecond);
BENCHMARK(Mutex)->Range(1<<10, 1<<14)->Unit(benchmark::kMillisecond);
BENCHMARK(Atomic)->Range(1<<10, 1<<14)->Unit(benchmark::kMillisecond);
BENCHMARK(AtomicRelaxed)->Range(1<<10, 1<<14)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();