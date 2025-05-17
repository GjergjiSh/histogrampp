#pragma once

#include <cassert>
#include <cstdint>
#include <cstdlib>

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

  uint64_t GetBinCount() const noexcept { return kBinCount; }
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
  }

  inline void UpdateBins(microseconds cycle_time) noexcept {
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

  // clang-format off
  static constexpr uint64_t kBinCount = (MaxValue - MinValue) / BinWidth;
  uint64_t distribution_[kBinCount] = {0}; // distribution of cycle times in bins
  microseconds last_cycle_time_ = 0;        // in microseconds
  microseconds lowest_cycle_time_ = 0;      // in microseconds
  microseconds highest_cycle_time_ = 0;     // in microseconds
  // clang-format on
};
