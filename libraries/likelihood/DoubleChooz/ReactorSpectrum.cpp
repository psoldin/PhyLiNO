#include "ReactorSpectrum.h"

namespace ana::dc {

  ReactorSpectrum::ReactorSpectrum(std::shared_ptr<io::Options> options): SpectrumBase(std::move(options)) {
    m_Oscillator = std::make_shared<Oscillator>(m_Options);
    m_ShapeCorrection = std::make_shared<ShapeCorrection>(m_Options, m_Oscillator);
    m_EnergyCorrection = std::make_shared<EnergyCorrection>(m_Options, m_ShapeCorrection);
  }

  bool ReactorSpectrum::check_and_recalculate(const ParameterWrapper &parameter) {
    return m_EnergyCorrection->check_and_recalculate(parameter);
  }

  std::span<const double> ReactorSpectrum::get_spectrum(params::dc::DetectorType type) const noexcept {
    return m_EnergyCorrection->get_spectrum(type);
  }
}
