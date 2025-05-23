#pragma once

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>

using microseconds = uint64_t; // Histogram uses microseconds for cycle times

template <uint64_t MinValue, uint64_t MaxValue, uint64_t BinWidth>
class Histogram {
  static_assert(MinValue < MaxValue, "MinValue must be less than MaxValue");
  static_assert(BinWidth > 0, "BinWidth must be greater than zero");

public:
  explicit Histogram() = default;
  ~Histogram() = default;

  Histogram(const Histogram &) = delete;
  Histogram &operator=(const Histogram &) = delete;
  Histogram(Histogram &&) = default;
  Histogram &operator=(Histogram &&) = default;

  void Update(microseconds cycle_time_us) noexcept {
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

  size_t GetBinCount() const noexcept { return kBinCount; }
  const uint64_t *GetBins() const noexcept { return distribution_; }

private:
  inline void UpdateCycleTimes(microseconds cycle_time) noexcept {
    if (cycle_time < lowest_cycle_time_ || lowest_cycle_time_ == 0) {
      lowest_cycle_time_ = cycle_time;
    }
    if (cycle_time > highest_cycle_time_) {
      highest_cycle_time_ = cycle_time;
    }
    last_cycle_time_ = cycle_time;

    if (std::getenv("DEBUG_CYCLE_TIMES")) {
      std::cout << "Updating cycle times: "
                << "Last: " << last_cycle_time_ << ", "
                << "Lowest: " << lowest_cycle_time_ << ", "
                << "Highest: " << highest_cycle_time_ << '\n';
    }
  }

  inline void UpdateBins(microseconds cycle_time) noexcept {
    size_t bin_index;
    if (cycle_time < MinValue) {
      std::cout << "Warning: cycle time " << cycle_time
                << " is below the minimum value of " << MinValue
                << ", adding to first bin.\n";
      bin_index = 0;
    } else if (cycle_time > MaxValue) {
      std::cout << "Warning: cycle time " << cycle_time
                << " is above the maximum value of " << MaxValue
                << ", adding to last bin.\n";
      bin_index = kBinCount - 1;
    } else {
      bin_index = (cycle_time - MinValue) / BinWidth;
    }

    distribution_[bin_index]++;
    if (std::getenv("DEBUG_BINS")) {
      std::cout << "Updating bins: cycle time " << cycle_time
                << " falls into bin index " << bin_index
                << ", current count: " << distribution_[bin_index] << '\n';
    }
  }

  // clang-format off
  static constexpr uint64_t kBinCount = (MaxValue - MinValue) / BinWidth;
  uint64_t distribution_[kBinCount] = {0}; // distribution of cycle times in bins
  microseconds last_cycle_time_ = 0;        // in microseconds
  microseconds lowest_cycle_time_ = 0;      // in microseconds
  microseconds highest_cycle_time_ = 0;     // in microseconds
  // clang-format on
};
