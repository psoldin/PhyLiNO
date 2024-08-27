#include "AccidentalBackground.h"

namespace ana::dc {

  inline bool check_parameter(const ana::dc::ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;
    using namespace params;

    bool is_valid = true;
    for (auto detector : {ND, FDI, FDII}) {
      for (int i = AccShape01; i <= AccShape38; ++i) {
        is_valid |= parameter.parameter_changed(index(detector, i));
      }
      is_valid |= parameter.parameter_changed(index(detector, BkgRAcc));
    }
    return is_valid;
  }

  void AccidentalBackground::check_and_recalculate_spectrum(const ana::dc::ParameterWrapper& parameter) {
    if (!check_parameter(parameter)) {
      return;
    }
    recalculate_spectra(parameter);
  }

  void AccidentalBackground::recalculate_spectra(const ana::dc::ParameterWrapper& parameter) noexcept {

  }

}

