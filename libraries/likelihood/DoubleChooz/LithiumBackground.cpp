#include "LithiumBackground.h"
#include "Calculate_Spectrum.h"

namespace ana::dc {

  inline bool parameter_changed(const ana::dc::ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;
    using namespace params;

    bool has_changed = parameter.range_changed(General::LiShape01, General::LiShape38 + 1);

    for (auto detector : {ND, FDI, FDII}) {
      has_changed |= parameter.parameter_changed(index(detector, BkgRLi));
    }

    return has_changed;
  }

  void LithiumBackground::check_and_recalculate_spectra(const ParameterWrapper& parameter) {
    if (parameter_changed(parameter)) {
      recalculate_spectra(parameter);
    }
  }

  void LithiumBackground::recalculate_spectra(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;
    using namespace params;

    for (auto detector : {ND, FDI, FDII}) {
      double      rate            = parameter[params::index(detector, BkgRLi)];
      const auto& shape           = m_SpectrumTemplate[detector];
      auto        shape_parameter = parameter.sub_range(General::LiShape01, General::LiShape38 + 1);
      const auto& covMatrix       = m_Options->dataBase().covariance_matrix(detector, io::SpectrumType::Lithium);
      auto&       result          = m_Cache[detector];

      calculate_spectrum<38>(rate,
                             shape,
                             shape_parameter,
                             covMatrix.block<38, 38>(0, 0),
                             result);
    }
  }

}  // namespace ana::dc
