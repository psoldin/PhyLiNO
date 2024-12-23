#pragma once

#include "ShapeCorrection.h"
#include <Options.h>
#include "../ParameterWrapper.h"
#include <Eigen/Core>

namespace ana::dc {

  class EnergyCorrection {
  public:
    explicit EnergyCorrection(std::shared_ptr<io::Options> options, std::shared_ptr<ShapeCorrection> shape_correction);

    ~EnergyCorrection() = default;

    void check_and_recalculate_spectra(const ParameterWrapper& parameter) noexcept;

  private:
    std::shared_ptr<io::Options> m_Options;
    std::shared_ptr<ShapeCorrection> m_ShapeCorrection;

    Eigen::Array<double, 80, 1> m_XPos;

    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 80, 1>> m_Cache;

    void calculate_spectra(const ParameterWrapper& parameter) noexcept;
  };
} // namespace ana::dc