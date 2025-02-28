#pragma once

#include "../SpectrumBase.h"
#include "Oscillator.h"
#include "EnergyCorrection.h"

namespace ana::dc {

  class ReactorSpectrum : public SpectrumBase {
  public:
    explicit ReactorSpectrum(std::shared_ptr<io::Options> options);

    bool check_and_recalculate(const ParameterWrapper& parameter) override;

    [[nodiscard]] std::span<const double> get_spectrum(params::dc::DetectorType type) const noexcept override;

    [[nodiscard]] const auto& oscillator() const noexcept { return m_Oscillator; }

    [[nodiscard]] const auto& shape_correction() const noexcept { return m_ShapeCorrection; }

    [[nodiscard]] const auto& energy_correction() const noexcept { return m_EnergyCorrection; }

  private:
    std::shared_ptr<Oscillator> m_Oscillator;
    std::shared_ptr<ShapeCorrection> m_ShapeCorrection;
    std::shared_ptr<EnergyCorrection> m_EnergyCorrection;
  };

}