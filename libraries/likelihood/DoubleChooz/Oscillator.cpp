#include "Oscillator.h"

namespace ana::dc {

  Oscillator::Oscillator(std::shared_ptr<io::Options> options)
    : m_Options(std::move(options)) {
  }

  void Oscillator::recalculate_spectra() {
    // Recalculate the spectra
  }

}  // namespace ana
