#include "AccidentalBackground.h"
#include "Calculate_Spectrum.h"
#include "Parameter.h"

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
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;
    using namespace params;

    for (auto detector : {ND, FDI, FDII}) {
      double      rate            = parameter[params::index(detector, BkgRAcc)];
      const auto& shape           = m_SpectrumTemplate[detector];
      auto        shape_parameter = parameter.sub_range(index(detector, AccShape01), index(detector, AccShape38) + 1);
      const auto& covMatrix       = m_Options->dataBase().covariance_matrix(detector, io::SpectrumType::Accidental);
      auto&       result          = m_Cache[detector];

      calculate_spectrum<38>(rate,
                             shape,
                             shape_parameter,
                             covMatrix.block<38, 38>(0, 0),
                             result);
    }
  }

}  // namespace ana::dc
