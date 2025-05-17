#include "histogram.h"
#include <random>

int main() {
  // clang-format off
  auto histogram_example = [](
    auto &&histogram, double mean, double stddev,
    uint64_t samples, const char *unit) {

    std::mt19937 gen(std::random_device{}());
    std::normal_distribution<double> normal_dist(mean, stddev);

    for (uint64_t i = 0; i < samples; ++i) {
      double sample = normal_dist(gen);
      microseconds cycle_time_us = static_cast<microseconds>(sample);
      histogram.Update(cycle_time_us);
    }

    histogram.Print(unit);
  };
  // clang-format on

  histogram_example(Histogram<2300, 2600, 10>(), 2450.0, 25.0, 1000000, "us");
  histogram_example(Histogram<9800, 10000, 10>(), 9900.0, 25.0, 1000000, "us");
}