#include "DCBackground.h"

namespace ana::dc {
  DCBackground::DCBackground(std::shared_ptr<io::Options> options)
    : SpectrumBase(options)
    , m_Accidental(options)
    , m_Lithium(options)
    , m_FastN(options) {}


  void DCBackground::refresh_cache() {
    using enum params::dc::DetectorType;

    for (auto detector : {ND, FDI, FDII}) {
      m_Cache[detector] = m_Accidental.get_spectrum(detector)
                          + m_Lithium.get_spectrum(detector)
                          + m_FastN.get_spectrum(detector);
    }
  }

  bool DCBackground::check_and_recalculate_spectra(const ParameterWrapper &parameter) {
    bool has_changed = false;
    has_changed |= m_Accidental.check_and_recalculate_spectra(parameter);
    has_changed |= m_Lithium.check_and_recalculate_spectra(parameter);
    has_changed |= m_FastN.check_and_recalculate_spectra(parameter);

    if (has_changed) {
      refresh_cache();
    }

    return has_changed;
  }

  const Eigen::Array<double, 44, 1> &DCBackground::get_spectrum(params::dc::DetectorType type) const noexcept {
    return m_Cache.at(type);
  }
}
