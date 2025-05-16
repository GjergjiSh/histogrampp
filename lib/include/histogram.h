#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#define NO_OFFSET 0

namespace xcp {
void capture();
void measure();
} // namespace xcp

namespace mock_periods {
static constexpr float kF1 = 24.0f;
static constexpr float kF2 = 24.3f;
static constexpr float kF3 = 24.6f;
static constexpr float kF4 = 25.0f;
static constexpr float kF5 = 25.3f;
static constexpr float kF6 = 25.6f;
} // namespace mock_periods

template <int64_t NumPoints, int64_t BinWidth, int64_t Offset> class Histogram {
public:
  explicit Histogram() = default;
  ~Histogram() = default;

  Histogram(const Histogram &) = delete;
  Histogram &operator=(const Histogram &) = delete;
  Histogram(Histogram &&) = default;
  Histogram &operator=(Histogram &&) = default;

  void update_period(float period) noexcept {
    last_period_ = current_period_;
    current_period_ = period;
  };

  void print(const char *unit) const noexcept {
    const auto verbose = getenv("HISTO_VERBOSE") != nullptr;
    if (verbose) {
      printf("Histogram:\n");
      for (int i = 0; i < NumPoints; ++i) {
        int start = Offset * BinWidth + i * BinWidth;
        int end = start + BinWidth;
        printf("[%d] [%d .. %d[ (%s) (%d)\n", i, start, end, unit, bins_[i]);
      }
      printf("Last period: %.2f %s\n", last_period_, unit);
      printf("Current period: %.2f %s\n", current_period_, unit);
    } else {
      for (int i = 0; i < NumPoints; ++i) {
        printf("%d\n ", bins_[i]);
      }
    }
  }

private:
  void calculate_bin_split() const noexcept {}

  uint32_t bins_[NumPoints] = {0};
  float last_period_ = 0.0f;
  float current_period_ = 0.0f;
};