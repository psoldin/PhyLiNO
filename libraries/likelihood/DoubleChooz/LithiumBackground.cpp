#include "LithiumBackground.h"
#include "Calculate_Spectrum.h"

namespace ana::dc {

  [[nodiscard]] inline bool check_parameters(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using namespace params;
    using namespace params::dc;

    bool recalculate = parameter.check_parameter_changed(General::LiShape01, General::LiShape38);

    for (auto detector : {ND, FDI, FDII}) {
      recalculate |= parameter.check_parameter_changed(index(detector, Detector::BkgRLi));
    }

    return recalculate;
  }

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

  bool LithiumBackground::check_and_recalculate_spectra(const ParameterWrapper& parameter) {
    bool has_changed = parameter_changed(parameter);
    if (has_changed) {
      recalculate_spectra(parameter);
    }
    return has_changed;
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
