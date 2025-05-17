#include "histogram.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <random>

class HistogramTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(HistogramTest, UpdateHistogram) {
  Histogram<230, 260, 1> histogram;

  std::vector<uint64_t> cycle_times;
  cycle_times.reserve(histogram.GetBinCount());
  for (uint64_t i = 230; i <= 260; ++i) {
    cycle_times.push_back(i);
  }

  for (const auto &cycle_time : cycle_times) {
    histogram.Update(cycle_time);
  }

  histogram.Print("ms * 100");
}

TEST_F(HistogramTest, UpdateHistogramWithSleep) {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;
  using std::chrono::steady_clock;

  Histogram<2300, 2600, 10> histogram;
  std::mt19937_64 rng{std::random_device{}()};
  std::uniform_real_distribution<double> dist{2430, 2550};

  for (uint64_t i = 0; i < 1000000; ++i) {
    double cycle_time = dist(rng);
    histogram.Update(cycle_time);
  }
  histogram.Print("us");
}
