#include "histogram.h"
#include <gtest/gtest.h>

class HistogramTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(HistogramTest, ExampleTest) {
  //  10 bins, each 5ms wide
  // 0...5ms, 5..10ms, 10..15ms, ..., 45..50ms
//   Histogram<10, 5> histogram;
//   histogram.print("ms");

  // Car use case
  // 23.98, 24.0, 24.31, 24,8, 25.0, 25.3 ms
  // 2398 2400 2431 2480 2500 2530 us | / 2
  // 1000...2000us, 2000..3000us, 4000..5000us, ..., 4500..5000us
  // Offset step numpoints
  // Histogram<3, 10> cycle_time;
}