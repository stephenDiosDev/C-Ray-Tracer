#pragma once

#include <chrono>

namespace temporal {
using high_resolution_clock = std::chrono::high_resolution_clock;
using milliseconds_t = std::chrono::milliseconds;
using seconds_t = std::chrono::seconds;
using minutes_t = std::chrono::minutes;
using hours_t = std::chrono::hours;

class Timer {
public:
  explicit Timer(bool run = false) {
    if (run)
      reset();
  }
  void reset() { m_start = high_resolution_clock::now(); }

  template <typename T, typename Rep = uint64_t> Rep elapsed() const {
    using std::chrono::duration_cast;
    return duration_cast<T>(high_resolution_clock::now() - m_start).count();
  }

  uint64_t milliseconds() const { return elapsed<milliseconds_t>(); }
  uint64_t seconds() const { return elapsed<seconds_t>(); }
  uint64_t minutes() const { return elapsed<minutes_t>(); }
  uint64_t hours() const { return elapsed<hours_t>(); }

private:
  high_resolution_clock::time_point m_start;
};
}