#include "ThreeFlavorOscillation.h"
#include <iostream>

namespace ana::dc {

  inline double get_cos4(const double xt13) noexcept {
    return std::pow(std::cos(std::asin(std::sqrt(xt13)) / 2.0), 4);
  }

  #pragma omp declare simd
  inline double pow_2(const double x) noexcept {
    return x * x;
  }

  ThreeFlavorOscillation::ThreeFlavorOscillation(double t13, double dmee, double t12, double dm21)
    : m_t13(t13)
    , m_dmee(dmee * 1.267)
    , m_t12(t12)
    , m_dm21(dm21 * 1.267)
    , m_cos413(get_cos4(m_t13)) {}

  double ThreeFlavorOscillation::oscillate_events(const OscillationData& data) const noexcept {
    using span_t = std::span<const double>;

    const span_t loe = data.LoverE;
    const span_t scl = data.scaling;

    const double cos4 = m_cos413 * m_t12;

    const std::size_t N = loe.size();

    double result = 0.0;

    #pragma omp simd reduction(+ : result)
    for (std::size_t i = 0; i < N; ++i) {
      const double t13Part = m_t13 * pow_2(sin(m_dmee * loe[i]));
      const double t12Part = cos4 * pow_2(sin(m_dm21 * loe[i]));
      result += scl[i] * (1 - t13Part - t12Part);
    }

    return result * get_MC_scaling_factor(params::dc::is_far_detector(data.type));
  }
}  // namespace ana::dc
