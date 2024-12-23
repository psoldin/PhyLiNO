#pragma once

#include "../SpectrumBase.h"
#include "Oscillator.h"
#include "EnergyCorrection.h"

namespace ana::dc {

  class ReactorSpectrum : public SpectrumBase {
  public:
    explicit ReactorSpectrum(std::shared_ptr<io::Options> options)
      : SpectrumBase(options)
      , m_Oscillator(options)
      , m_EnergyCorrection(options) {}

    [[nodiscard]] bool check_and_recalculate_spectra(const ParameterWrapper& parameter) override;

    [[nodiscard]] const Eigen::Array<double, 44, 1>& get_spectrum(params::dc::DetectorType type) const noexcept override;

  private:
    void recalculate_oscillation_spectra(const ParameterWrapper& parameter) noexcept {
      m_Oscillator.recalculate_spectra(parameter);
    }

    void recalculate_energy_correction(const ParameterWrapper& parameter) noexcept {
      m_EnergyCorrection.calculate_energy_correction(parameter, m_Oscillator);
    }

    void recalculate_shape_correction(const ParameterWrapper& parameter) noexcept {

    }

    Oscillator m_Oscillator;
    EnergyCorrection m_EnergyCorrection;
  };

}