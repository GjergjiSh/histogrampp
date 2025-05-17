#pragma once

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>

using microsecond = uint64_t; // Histogram uses microseconds for cycle times

template <microsecond MinValue, microsecond MaxValue, microsecond BinWidth>
class Histogram {
  static_assert(MinValue < MaxValue, "MinValue must be less than MaxValue");
  static_assert(BinWidth > 0, "BinWidth must be greater than zero");

public:
  static constexpr uint64_t kBinCount = (MaxValue - MinValue) / BinWidth;
  explicit Histogram() = default;
  ~Histogram() = default;

  Histogram(const Histogram &) = delete;
  Histogram &operator=(const Histogram &) = delete;
  Histogram(Histogram &&) = default;
  Histogram &operator=(Histogram &&) = default;

  inline void Update(microsecond cycle_time_us) noexcept {
    UpdateCycleTimes(cycle_time_us);
    UpdateBins(cycle_time_us);
  }

  void Print(const char *unit = "") const noexcept {
    if (std::getenv("PRINT_VERBOSE")) {
      std::cout << "Histogram:\n";
      for (uint32_t i = 0; i < kBinCount; ++i) {
        uint64_t start = MinValue + i * BinWidth;
        uint64_t end = start + BinWidth;
        std::cout << "[" << i << "] [" << start << " .. " << end << "[ ("
                  << unit << ") (" << distribution_[i] << ")\n";
      }
      // clang-format off
      std::cout << "Last cycle time: " << last_cycle_time_ << ' ' << unit << '\n';
      std::cout << "Lowest cycle time: " << lowest_cycle_time_ << ' ' << unit << '\n';
      std::cout << "Highest cycle time: " << highest_cycle_time_ << ' ' << unit << '\n';
      // clang-format on
    } else {
      for (uint64_t i = 0; i < kBinCount; ++i) {
        std::cout << distribution_[i] << '\n';
      }
    }
  }

  inline const uint64_t *GetBins() const noexcept { return distribution_; }

private:
  inline void UpdateCycleTimes(microsecond cycle_time) noexcept {
    if (cycle_time < lowest_cycle_time_ || lowest_cycle_time_ == 0) {
      lowest_cycle_time_ = cycle_time;
    }
    if (cycle_time > highest_cycle_time_) {
      highest_cycle_time_ = cycle_time;
    }
    last_cycle_time_ = cycle_time;
  }

  inline void UpdateBins(microsecond cycle_time) noexcept {
    size_t bin_index;
    // Underflow goes in the first bin
    if (cycle_time < MinValue) {
      bin_index = 0;
      // Overflow goes in the last bin
    } else if (cycle_time > MaxValue) {
      bin_index = kBinCount - 1;
    } else {
      bin_index = (cycle_time - MinValue) / BinWidth;
    }

    distribution_[bin_index]++;
  }

  uint64_t distribution_[kBinCount] = {0};
  microsecond last_cycle_time_ = 0;
  microsecond lowest_cycle_time_ = 0;
  microsecond highest_cycle_time_ = 0;
};
