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

  void correlate_parameters(const io::Options& options, std::span<double> parameters) {
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;
    using namespace params;

    // FDI and FDII lithium background rates are fully correlated
    parameters[index(FDI, BkgRLi)] = parameters[index(FDII, BkgRLi)];

    const auto& dco = options.double_chooz().dataBase();

    {  // Correlate Energy Parameters
      // EnergyA is fully correlated among all detectors
      constexpr std::array<int, 7> energy_indices = {EnergyA,
                                                     index(FDI, EnergyB),
                                                     index(ND, EnergyB),
                                                     index(FDII, EnergyB),
                                                     index(FDI, EnergyC),
                                                     index(ND, EnergyC),
                                                     index(FDII, EnergyC)};

      TVectorD energy_correlations(7);

      for (std::size_t i = 0; i < energy_indices.size(); ++i) {
        energy_correlations[i] = parameters[energy_indices[i]];
      }

      energy_correlations *= dco.energy_correlation_matrix();

      for (std::size_t i = 0; i < energy_indices.size(); ++i) {
        parameters[energy_indices[i]] = energy_correlations[i];
      }
    }

    {
      constexpr std::array mcNorm_indices = {index(FDI, MCNorm),
                                             index(ND, MCNorm),
                                             index(FDII, MCNorm)};

      TVectorD mcNorm_correlations(3);
      for (std::size_t i = 0; i < mcNorm_indices.size(); ++i) {
        mcNorm_correlations[i] = parameters[mcNorm_indices[i]];
      }

      mcNorm_correlations *= dco.mcNorm_correlation_matrix();

      for (std::size_t i = 0; i < mcNorm_indices.size(); ++i) {
        parameters[mcNorm_indices[i]] = mcNorm_correlations[i];
      }
    }
    {
      const auto& covMatrix = dco.interDetector_correlation_matrix();
      TVectorD    reactor_correlations(3);
      for (int i = NuShape01; i <= NuShape43; ++i) {
        reactor_correlations[0] = parameters[index(FDI, i)];
        reactor_correlations[1] = parameters[index(ND, i)];
        reactor_correlations[2] = parameters[index(FDII, i)];

        reactor_correlations *= covMatrix;

        parameters[index(FDI, i)]  = reactor_correlations[0];
        parameters[index(ND, i)]   = reactor_correlations[1];
        parameters[index(FDII, i)] = reactor_correlations[2];
      }
    }
  }

  double DCLikelihood::calculate_likelihood(const double* parameter) {
    m_Parameter.reset_parameter(parameter);
    if (m_Options->inputOptions().double_chooz().reactor_split()) {
      return calculate_reactor_split_likelihood(m_Parameter);
    }
    return calculate_default_likelihood(m_Parameter);
  }

  double DCLikelihood::calculate_off_off_likelihood(const Eigen::Array<double, 44, 1>& bkg, params::dc::DetectorType detector) {
    constexpr int nBins = 44;
    using map_t         = Eigen::Map<const Eigen::Array<double, 44, 1>>;

    // Get the off-off data
    map_t off_off_data(get_off_off_data(detector).data(), nBins);

    // Get the on and off lifetimes
    const double on_lifetime  = m_Options->double_chooz().dataBase().on_lifetime(detector);
    const double off_lifetime = m_Options->double_chooz().dataBase().off_lifetime(detector);

    // Rescale the background to the off time
    auto off_off_bkg = (off_lifetime / on_lifetime) * bkg;

    // Exclude the bins up to 3 MeV due to residual neutrinos
    constexpr std::size_t idx = std::distance(io::dc::Constants::EnergyBinXaxis.cbegin(), std::ranges::lower_bound(io::dc::Constants::EnergyBinXaxis, 3.0));

    // Calculate Poisson Likelihood
    return -2.0 * (off_off_data * off_off_bkg.log() - off_off_bkg).tail<nBins - idx>().sum();
  }

  double DCLikelihood::calculate_default_likelihood(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;

    double likelihood = 0.0;

    constexpr int nBins = 44;

    for (auto detector : {ND, FDI, FDII}) {
      using map_t   = Eigen::Map<const Eigen::Array<double, nBins, 1>>;
      using array_t = Eigen::Array<double, nBins, 1>;

      map_t acc(m_Accidental.get_spectrum(detector).data(), nBins);
      map_t li(m_Lithium.get_spectrum(detector).data(), nBins);
      map_t fastN(m_FastN.get_spectrum(detector).data(), nBins);
      map_t dnc(m_DNC.get_spectrum(detector).data(), nBins);
      map_t reactor(m_Reactor.get_spectrum(detector).data(), nBins);

      const array_t bkg = acc + li + fastN + dnc;

      map_t data(get_measurement_data(detector).data(), nBins);

      const double mcNorm = parameter[params::index(detector, params::dc::Detector::MCNorm)];

      auto prediction = bkg + (mcNorm * reactor);

      // Calculate Poisson Likelihood
      likelihood += -2.0 * (data * prediction.log() - prediction).sum();

      if (detector == ND || detector == FDII) {
        likelihood += calculate_off_off_likelihood(bkg, detector);
      }
    }

    return std::isfinite(likelihood) ? likelihood : 1.0e25;
  }
}  // namespace ana::dc
