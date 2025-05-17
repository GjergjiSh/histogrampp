#pragma once

#include <cstdint>
#include <cstdlib>
#include <iostream>

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

  void Update(uint64_t cycle_time) noexcept {
    UpdateCycleTimes(cycle_time);
    UpdateBins(cycle_time);
  }

  void Print(const char *unit) const noexcept {
    if (std::getenv("DEBUG_CYCLE_TIMES")) {
      std::cout << "Histogram:\n";
      for (uint32_t i = 0; i < kBinCount; ++i) {
        uint64_t start = MinValue + (BinWidth + i * BinWidth);
        uint64_t end = start + BinWidth;
        std::cout << "[" << i << "] [" << start << " .. " << end << "[ ("
                  << unit << ") (" << bins_[i] << ")\n";
      }
      // clang-format off
      std::cout << "Last cycle time: " << last_cycle_time_ << ' ' << unit << '\n';
      std::cout << "Lowest cycle time: " << lowest_cycle_time_ << ' ' << unit << '\n';
      std::cout << "Highest cycle time: " << highest_cycle_time_ << ' ' << unit << '\n';
      // clang-format on
    } else {
      for (uint32_t i = 0; i < kBinCount; ++i) {
        std::cout << bins_[i] << '\n';
      }
    }
  }

  size_t GetBinCount() const noexcept { return kBinCount; }

private:
  void UpdateCycleTimes(uint64_t cycle_time) noexcept {
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

  void UpdateBins(uint64_t cycle_time) noexcept {
    if (cycle_time < MinValue || cycle_time > MaxValue) {
      std::cout << "Warning: cycle time " << cycle_time
                << " out of bounds for histogram with min " << MinValue
                << " and max " << MaxValue << '\n';
      return;
    }

    const auto bin_index = kBinCount - (MaxValue - cycle_time) / BinWidth;
    if (bin_index < kBinCount) {
      bins_[bin_index]++;
    } else {
      std::cout << "Error: bin index out of range: " << bin_index
                << ", for cycle time: " << cycle_time << std::endl;
    }

    if (std::getenv("DEBUG_BINS")) {
      std::cout << "Updating bins: cycle time " << cycle_time
                << " falls into bin index " << bin_index
                << ", current count: " << bins_[bin_index] << '\n';
    }
  }

  static constexpr uint64_t kBinCount = (MaxValue - MinValue) / BinWidth;
  uint32_t bins_[kBinCount] = {0};
  uint64_t last_cycle_time_ = 0;
  uint64_t lowest_cycle_time_ = 0;
  uint64_t highest_cycle_time_ = 0;
};
