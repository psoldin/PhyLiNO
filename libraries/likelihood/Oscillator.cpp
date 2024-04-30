#include "Oscillator.h"

namespace ana {

  Oscillator::Oscillator(const io::Options& options)
    : m_Options(options) {
    using enum parms::dc::DetectorType;

    for (auto detector : {FDI, FDII, ND}) {
      // Get the reactor data for the detector
      const auto& reactor_data = m_Options.database().reactor_data(detector);
    }
  }

  void Oscillator::recalculate_spectra() {
    // Recalculate the spectra
  }

}  // namespace ana
