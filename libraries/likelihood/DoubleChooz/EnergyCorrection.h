#pragma once

#include "Oscillator.h"
#include <Eigen/Core>

namespace ana::dc {

  class EnergyCorrection {
  public:
    explicit EnergyCorrection(std::shared_ptr<io::Options> options);

    ~EnergyCorrection() = default;

    void calculate_energy_correction(const ParameterWrapper& parameter, const Oscillator& oscillator) noexcept;

  private:
    std::shared_ptr<io::Options> m_Options;
    Eigen::Array<double, 80, 1> m_XPos;

    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 80, 1>> m_Cache;
  };
} // namespace ana::dc