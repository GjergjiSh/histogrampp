#include "histogram.h"
#include <gtest/gtest.h>

class HistogramTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(HistogramTest, ExampleTest) {
  Histogram<30, 10> histogram;
  histogram.print("ms");
}

TEST_F(HistogramTest, PrintWithOffset) {
  Histogram<30, 10> histogram;
  histogram.print("ms");
}

TEST_F(HistogramTest, UpdatePeriod) {
  Histogram<30, 10> histogram;

  // clang-format off
  std::vector<double> values = {
    23.0, 23.1, 23.2, 23.3, 23.4,
    23.5, 23.6, 23.7, 23.8, 23.9,
    24.0, 24.1, 24.2, 24.3, 24.4,
    24.5, 24.6, 24.7, 24.8, 24.9,
    25.0, 25.1, 25.2, 25.3, 25.4,
    25.5, 25.6, 25.7, 25.8, 25.9,
    26.0
  };
  // clang-format on

  for (const auto &v : values) {
    histogram.update(v);
  }

  histogram.print("ms");
}