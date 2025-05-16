#pragma once

#include <cstdint>
#include <cstdio>

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

template <int64_t Offset, int64_t Step, int64_t NumPoints> class Histogram {
public:
  explicit Histogram() = default;
  ~Histogram() = default;

  Histogram(const Histogram &) = delete;
  Histogram &operator=(const Histogram &) = delete;
  Histogram(Histogram &&) = default;
  Histogram &operator=(Histogram &&) = default;

  void update_period(float period) noexcept;
  void print(const char *unit) const noexcept {

  }

private:
  void calculate_bin_split() const noexcept {}

//   uint32_t bins_[BinCount] = {0};
  float last_period_ = 0.0f;
  float current_period_ = 0.0f;
};

// What does each bin represent?
// A range of periods
// The periods are numbers
// We need to calculate which bin a period falls into
// We need to calculate how to split the BinCount into ranges