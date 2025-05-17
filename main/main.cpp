#include "histogram.h"
#include <random>

#include <unistd.h>

int main() {
  Histogram<2300, 2600, 10> histogram;
  std::mt19937 gen(std::random_device{}());
  std::normal_distribution<double> normal_dist(2450.0, 25.0);

  for (uint64_t i = 0; i < 1000000; ++i) {
    double sample = normal_dist(gen);
    microseconds cycle_time_us = static_cast<microseconds>(sample);
    histogram.Update(cycle_time_us);
  }

  histogram.Print("us");
}