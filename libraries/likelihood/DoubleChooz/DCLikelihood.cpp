#include "DCLikelihood.h"

namespace ana::dc {

  inline double calculate_poisson_likelihood(const return_t& data, const return_t& signal, const return_t& bkg) noexcept {
    auto model = bkg + signal;
    return -2.0 * (data * model.log() - model).sum();
  }

  void DCLikelihood::recalculate_spectra(const ParameterWrapper& parameter) noexcept {
    m_Background.check_and_recalculate_spectra(m_Parameter);
  }

  double DCLikelihood::calculate_likelihood(const double* parameter) {
    m_Parameter.reset_parameter(parameter);
    if (m_Options->inputOptions().use_reactor_split()) {
      return calculate_reactor_split_likelihood(parameter);
    } else {
      return calculate_default_likelihood(parameter);
    }
  }

  double DCLikelihood::calculate_default_likelihood(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;

    double likelihood = 0.0;

    std::vector<params::dc::DetectorType> detectors = {ND, FDI, FDII};

    for (auto detector : {ND, FDI, FDII}) {
      const auto& bkg    = m_Background.get_spectrum(detector);
      const auto& data   = get_measurement_data(detector);
      const auto& signal = m_Background.get_spectrum(detector);  // TODO

      likelihood += calculate_poisson_likelihood(data, signal, bkg);

      if (detector == ND || detector == FDII) {
        likelihood += calculate_off_off_likelihood(bkg, detector);
      }
    }

    return likelihood;
  }
}  // namespace ana::dc
