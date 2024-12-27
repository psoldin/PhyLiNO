#pragma once

// io includes
#include <Options.h>

// includes
#include "ShapeCorrection.h"
#include "../ParameterWrapper.h"
#include "../SpectrumBase.h"

// Eigen includes
#include <Eigen/Core>

namespace ana::dc {

  class EnergyCorrection : public SpectrumBase {
  public:
    explicit EnergyCorrection(std::shared_ptr<io::Options> options, std::shared_ptr<ShapeCorrection> shape_correction);

    ~EnergyCorrection() override = default;

    [[nodiscard]] bool check_and_recalculate(const ParameterWrapper& parameter) noexcept override;

    [[nodiscard]] std::span<const double> get_spectrum(params::dc::DetectorType type) const noexcept override;

  private:
    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 80, 1>> m_Cache;
    Eigen::Array<double, 80, 1> m_XPos;
    std::shared_ptr<ShapeCorrection> m_ShapeCorrection;

    void calculate_spectra(const ParameterWrapper& parameter) noexcept;
  };
} // namespace ana::dc