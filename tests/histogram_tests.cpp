#include "histogram.h"
#include <gtest/gtest.h>

class HistogramTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(HistogramTest, ExampleTest) {
  Histogram<30, 10, NO_OFFSET> histogram;
  histogram.print("ms");
}

TEST_F(HistogramTest, PrintWithOffset) {
  Histogram<30, 10, 2> histogram;
  histogram.print("ms");
}

TEST_F(HistogramTest, UpdatePeriod) {
    Histogram<30, 10, NO_OFFSET> histogram;
}