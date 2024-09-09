#include "FastNBackground.h"
#include "Calculate_Spectrum.h"
#include "Parameter.h"

namespace ana::dc {

  inline bool parameter_changed(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;
    using namespace params;

    bool has_changed = false;

    for (auto detector : {ND, FDI, FDII}) {
      has_changed |= parameter.range_changed(index(detector, FNSMShape01), index(detector, FNSMShape44) + 1);
      has_changed |= parameter.parameter_changed(index(detector, BkgRFNSM));
    }

    return has_changed;
  }

  bool FastNBackground::check_and_recalculate_spectra(const ana::dc::ParameterWrapper &parameter) {
    bool has_changed = parameter_changed(parameter);
    if (has_changed) {
      recalculate_spectra(parameter);
    }
    return has_changed;
  }

  void FastNBackground::recalculate_spectra(const ana::dc::ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;
    using namespace params;

    for (auto detector : {ND, FDI, FDII}) {
      double      rate            = parameter[params::index(detector, BkgRFNSM)];
      const auto& shape           = m_SpectrumTemplate[detector];
      auto        shape_parameter = parameter.sub_range(index(detector, FNSMShape01), index(detector, FNSMShape44) + 1);
      const auto& covMatrix       = m_Options->dataBase().covariance_matrix(detector, io::SpectrumType::FastN);
      auto&       result          = m_Cache[detector];

      calculate_spectrum<44>(rate,
                             shape,
                             shape_parameter,
                             covMatrix,
                             result);
    }
  }

}  // namespace ana::dc
