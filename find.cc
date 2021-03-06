#include <benchmark/benchmark.h>
#include <vector>
#include <unordered_set>
#include <set>

class FindFixture : public benchmark::Fixture {
public:
  void SetUp(const ::benchmark::State& state) {
      for(int i = 0; i<ELEMENT_COUNT; ++i) {
          v.push_back(i);
          us.insert(i);
          s.insert(i);
      }
  }

  void TearDown(const ::benchmark::State& state) {
  }

protected:
    const int ELEMENT_COUNT = 10000;
    std::vector<int> v;
    std::unordered_set<int> us;
    std::set<int> s;
};

BENCHMARK_DEFINE_F(FindFixture, FindInUnorderedSet)(benchmark::State& state) {
    while(state.KeepRunning()) {
        for(int i = 0; i<state.range(0); ++i) {
            us.find(rand() % ELEMENT_COUNT);
        }
    }
}

BENCHMARK_DEFINE_F(FindFixture, FindInSet)(benchmark::State& state) {
    while (state.KeepRunning()) {
        for (int i = 0; i < state.range(0); ++i) {
            s.find(rand() % ELEMENT_COUNT);
        }
    }
}

BENCHMARK_REGISTER_F(FindFixture, FindInUnorderedSet)->Range(2, 1<<14)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(FindFixture, FindInSet)->Range(2, 1 << 14)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();