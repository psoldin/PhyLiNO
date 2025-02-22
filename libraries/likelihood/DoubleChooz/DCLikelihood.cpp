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
   * This assumes that the sum over background and signal prediction is larger than zero.
   *
   * @param data A span of observed data values.
   * @param signal A span of expected signal values.
   * @param bkg A span of expected background values.
   * @return The calculated Poisson likelihood.
   */
  inline double calculate_poisson_likelihood(std::span<const double> data, std::span<const double> signal, std::span<const double> bkg) noexcept {
    double return_value = 0.0;
    for (size_t i = 0; i < data.size(); ++i) {
      const double model_i = signal[i] + bkg[i];
      return_value += data[i] * std::log(model_i) - model_i;
    }

    return -2.0 * return_value;
  }

  bool DCLikelihood::recalculate_spectra(const ParameterWrapper& parameter) noexcept {

    bool recalculate = false;

    std::array<SpectrumBase*, 5> components = {&m_Accidental, &m_Lithium, &m_FastN, &m_DNC, &m_Reactor};
    for (auto* component : components) {
      recalculate |= component->check_and_recalculate(parameter);
    }

    return recalculate;
  }

  double DCLikelihood::calculate_likelihood(const double* parameter) {
    m_Parameter.reset_parameter(parameter);
    if (m_Options->inputOptions().double_chooz().reactor_split()) {
      return calculate_reactor_split_likelihood(m_Parameter);
    }
    return calculate_default_likelihood(m_Parameter);
  }

  double DCLikelihood::calculate_default_likelihood(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;

    double likelihood = 0.0;

    std::array<double, 45> bkg{};

    for (auto detector : {ND, FDI, FDII}) {
      std::ranges::fill(bkg, 0.0);
      auto acc     = m_Accidental.get_spectrum(detector);
      auto li      = m_Lithium.get_spectrum(detector);
      auto fastN   = m_FastN.get_spectrum(detector);
      auto dnc     = m_DNC.get_spectrum(detector);
      auto reactor = m_Reactor.get_spectrum(detector);

      for (size_t i = 0; i < acc.size(); ++i) {
        bkg[i] = acc[i] + li[i] + fastN[i] + dnc[i];
      }

      auto data = get_measurement_data(detector);

      likelihood += calculate_poisson_likelihood(data, reactor, bkg);

      if (detector == ND || detector == FDII) {
        likelihood += calculate_off_off_likelihood(bkg, detector);
      }
    }

    return likelihood;
  }
}  // namespace ana::dc
