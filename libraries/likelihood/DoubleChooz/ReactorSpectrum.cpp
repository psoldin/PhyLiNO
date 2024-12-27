#include "ReactorSpectrum.h"

namespace ana::dc {

  bool ReactorSpectrum::check_and_recalculate(const ParameterWrapper &parameter) {
    return m_EnergyCorrection->check_and_recalculate(parameter);
  }

  std::span<const double> ReactorSpectrum::get_spectrum(params::dc::DetectorType type) const noexcept {
    return m_EnergyCorrection->get_spectrum(type);
  }
}
