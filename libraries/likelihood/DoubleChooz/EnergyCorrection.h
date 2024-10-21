#pragma once

#include "Oscillator.h"

namespace ana::dc {

  class EnergyCorrection {
  public:
    explicit EnergyCorrection(std::shared_ptr<io::Options> options)
    : m_Options(std::move(options)) { }

    ~EnergyCorrection() = default;

    void calculate_energy_correction(const ParameterWrapper& parameter, const Oscillator& oscillator) noexcept;

  private:
    std::shared_ptr<io::Options> m_Options;

    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 80, 1>> m_Cache;
  };
} // namespace ana::dc