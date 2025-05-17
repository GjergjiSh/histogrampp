#include "histogram.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <random>

class HistogramTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(HistogramTest, UniformDistribution) {
  Histogram<100, 200, 1> histogram;
  auto bin_count = histogram.kBinCount;
  EXPECT_EQ(bin_count, 100);

  for (uint64_t i = 100; i <= 200; ++i) {
    histogram.Update(i);
  }

  auto bins = histogram.GetBins();
  for (size_t i = 0; i < bin_count; ++i) {
    EXPECT_EQ(bins[i], 1u);
  }

  histogram.Print();
}

TEST_F(HistogramTest, NormalDistribution) {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;
  using std::chrono::steady_clock;

  Histogram<2300, 2600, 10> histogram;
  auto bin_count = histogram.kBinCount;
  EXPECT_EQ(bin_count, 30);

  // Simulate a normal(ish) distribution of cycle times
  std::mt19937_64 rng{std::random_device{}()};
  std::uniform_real_distribution<double> dist{2.430, 2.550};

  for (uint64_t i = 0; i < 1000000; ++i) {
    double cycle_time = dist(rng);
    uint64_t cycle_time_ms = static_cast<uint64_t>(cycle_time * 1000);
    histogram.Update(cycle_time_ms);
  }

  histogram.Print();
}

TEST_F(HistogramTest, TestManualDistribution) {
  Histogram<2300, 2600, 10> histogram;
  auto bin_count = histogram.kBinCount;
  EXPECT_EQ(bin_count, 30);

  static const microseconds kExpectedCycleTime = 2430;
  histogram.Update(kExpectedCycleTime);
  auto bins = histogram.GetBins();
  // 2430 - 2300 = 130, 130 / 10 = 13
  EXPECT_EQ(bins[13], 1);

  histogram.Print();
}

TEST_F(HistogramTest, InvalidDistribution) {
  Histogram<230, 260, 1> histogram;

  // Test with a cycle time below the minimum value
  histogram.Update(229);
  // Test with a cycle time above the maximum value
  histogram.Update(261);

  histogram.Print();
}
