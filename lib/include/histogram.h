#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>

template <int64_t BinCount, int64_t BinWidth> class Histogram {
public:
  explicit Histogram() = default;
  ~Histogram() = default;

  Histogram(const Histogram &) = delete;
  Histogram &operator=(const Histogram &) = delete;
  Histogram(Histogram &&) = default;
  Histogram &operator=(Histogram &&) = default;

  void update(double cycle_time) noexcept {
    const size_t bin_index = static_cast<size_t>(cycle_time / BinWidth);

    if (getenv("HISTO_VERBOSE")) {
      printf("Updating histogram with cycle time: %.2f, "
             "bin index: %zu, bin width: %ld\n",
             cycle_time, bin_index, BinWidth);
    }

    if (bin_index >= 0 && bin_index < BinCount) {
      bins_[bin_index]++;
    } else {
      printf("Warning: Period %.2f out of bounds for "
             "histogram with %ld points and bin width %ld\n",
             cycle_time, BinCount, BinWidth);
    }

    if (cycle_time < lowest_cycle_time_ || lowest_cycle_time_ == 0) {
      lowest_cycle_time_ = cycle_time;
    }

    if (cycle_time > lowers_cycle_time) {
      lowers_cycle_time = cycle_time;
    }

    last_cycle_time_ = cycle_time;
  };

  void print(const char *unit) const noexcept {
    if (getenv("HISTO_VERBOSE")) {
      printf("Histogram:\n");
      for (int i = 0; i < BinCount; ++i) {
        int start = BinWidth + i * BinWidth;
        int end = start + BinWidth;
        printf("[%d] [%d .. %d[ (%s) (%d)\n", i, start, end, unit, bins_[i]);
      }
      printf("Last cycle time: %.2f %s\n", last_cycle_time_, unit);
      printf("Lowest cycle time: %.2f %s\n", lowest_cycle_time_, unit);
      printf("Highest cycle time: %.2f %s\n", lowers_cycle_time, unit);
    } else {
      for (int i = 0; i < BinCount; ++i) {
        printf("%d\n ", bins_[i]);
      }
    }
  }

private:
  uint32_t bins_[BinCount] = {0};
  double last_cycle_time_ = 0;
  double lowest_cycle_time_ = 0;
  double lowers_cycle_time = 0;
};