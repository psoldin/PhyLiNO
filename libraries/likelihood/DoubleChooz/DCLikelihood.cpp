#include "DCLikelihood.h"

namespace ana::dc {

  /**
   * @brief Calculates the Poisson likelihood for given data, signal, and background.
   *
   * This function computes the Poisson likelihood for a given set of observed data,
   * expected signal, and background. The likelihood is calculated using the formula:
   *
   * \f[
   * \mathcal{L} = -2 \sum_{i} \left( data_i \log(model_i) - model_i \right)
   * \f]
   *
   * where \f$ model_i = signal_i + bkg_i \f$.
   *
   * @param data A span of observed data values.
   * @param signal A span of expected signal values.
   * @param bkg A span of expected background values.
   * @return The calculated Poisson likelihood.
   */
  inline double calculate_poisson_likelihood(std::span<const double> data, std::span<const double> signal, std::span<const double> bkg) noexcept {
    auto model = bkg + signal;

    double return_value = 0.0;
    for (size_t i = 0; i < data.size(); ++i) {
      const double model_i = signal[i] + bkg[i];
      return_value += data[i] * std::log(model_i) - model_i;
    }

    return -2.0 * return_value;
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
