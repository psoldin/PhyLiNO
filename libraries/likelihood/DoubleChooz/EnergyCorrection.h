#pragma once

#include "ShapeCorrection.h"
#include <Options.h>
#include "../ParameterWrapper.h"
#include <Eigen/Core>

namespace ana::dc {

  class EnergyCorrection : public SpectrumBase {
  public:
    explicit EnergyCorrection(std::shared_ptr<io::Options> options, std::shared_ptr<ShapeCorrection> shape_correction);

    ~EnergyCorrection() override = default;

    [[nodiscard]] bool check_and_recalculate(const ParameterWrapper& parameter) noexcept;

  private:
    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 80, 1>> m_Cache;
    Eigen::Array<double, 80, 1> m_XPos;

    std::shared_ptr<ShapeCorrection> m_ShapeCorrection;



    void calculate_spectra(const ParameterWrapper& parameter) noexcept;
  };
} // namespace ana::dc