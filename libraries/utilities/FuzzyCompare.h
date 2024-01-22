#pragma once

// STL includes
#include <cmath>
#include <type_traits>

// TODO Documentation

namespace utilities {

  template <typename T>
  concept is_float_double = std::is_same_v<float, std::decay_t<T>> || std::is_same_v<double, std::decay_t<T>>;

  template <is_float_double T>
  [[nodiscard]] __attribute__((always_inline)) inline bool fuzzyIsNull(T&& value) noexcept {
    if constexpr (std::is_same_v<typename std::decay_t<T>, double>) {
      return std::abs(value) <= 1e-12;
    } else {
      return std::abs(value) <= 0.00001f;
    }
  }

  template <is_float_double T1, is_float_double T2>
  [[nodiscard]] __attribute__((always_inline)) inline bool fuzzyCompare(T1&& a, T2&& b) noexcept {
    return fuzzyIsNull(a - b);
  }

}  // namespace utilities
