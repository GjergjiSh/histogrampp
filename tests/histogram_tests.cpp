#include "histogram.h"
#include <gtest/gtest.h>

class HistogramTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(HistogramTest, ExampleTest) {
  Histogram<10, 5> histogram;
  histogram.print();
}