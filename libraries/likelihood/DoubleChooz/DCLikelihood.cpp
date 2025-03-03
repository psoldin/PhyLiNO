#include "DCLikelihood.h"

#include <ranges>

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

  void DCLikelihood::initialize_measurement_data() {
    if (m_Options->inputOptions().double_chooz().use_data()) {
      read_measurement_data();
    } else {
      generate_measurement_data();
    }
  }

  void DCLikelihood::read_measurement_data() {
    throw std::runtime_error("Not implemented");
  }

  inline void print_array(std::span<const double> data, std::string_view name) {
    std::cout << name << ":\n";
    for (const auto& d : data) {
      std::cout << d << ' ';
    }
    std::cout << '\n';
  }

  void DCLikelihood::generate_measurement_data() {
    std::vector<double> parameter(params::number_of_parameters(), 0.0);

    const auto& pv = m_Options->inputOptions().input_parameters().parameters();

    for (std::size_t i = 0; i < parameter.size(); ++i) {
      parameter[i] = pv[i].value();
    }

    m_Parameter.reset_parameter(parameter.data());

    std::cout << "First check and recalculate\n";
    m_Reactor.check_and_recalculate(m_Parameter);
    std::cout << "After check and recalculate\n";

    using enum params::dc::DetectorType;

    for (auto detector : {ND, FDI, FDII}) {
      std::span<const double> spectrum = m_Reactor.get_spectrum(detector);
      print_array(spectrum, "Reactor Spectrum " + params::dc::get_detector_name(detector));
      std::array<double, 44> array{};
      std::ranges::copy(spectrum, array.begin());
      m_MeasurementData[detector] = array;

      if (detector == ND || detector == FDII) {
        std::array<double, 44> off_off_data{};
        std::ranges::fill(off_off_data, 0.0);
        m_OffOffData[detector] = off_off_data;
      }
    }
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

  DCLikelihood::DCLikelihood(std::shared_ptr<io::Options> options)
    : Likelihood(std::move(options))
    , m_Parameter(params::number_of_parameters(), m_Options)
    , m_Accidental(m_Options)
    , m_Lithium(m_Options)
    , m_FastN(m_Options)
    , m_DNC(m_Options)
    , m_Reactor(m_Options) {
    initialize_measurement_data();
    // m_Components = {&m_Accidental, &m_Lithium, &m_FastN, &m_DNC, &m_Reactor};
    m_Components = {&m_Reactor, &m_Accidental};
  }

  double DCLikelihood::calculate_likelihood(const double* parameter) {
    m_Parameter.reset_parameter(parameter);

    for (auto* component : m_Components) {
      std::cout << "Component: " << component << '\n';
      component->check_and_recalculate(m_Parameter);
    }

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
    constexpr std::size_t idx = std::distance(io::dc::Constants::EnergyBinXaxis.cbegin(),
                                              std::ranges::lower_bound(io::dc::Constants::EnergyBinXaxis, 3.0));

    const Eigen::Array<double, 44, 1> off_off_llh = -2.0 * (off_off_data * off_off_bkg.log() - off_off_bkg);

    // Calculate Poisson Likelihood
    return -2.0 * off_off_llh.tail(nBins - idx).sum();
    // return -2.0 * (off_off_data * off_off_bkg.log() - off_off_bkg).tail(nBins - idx).sum();
  }

  double DCLikelihood::calculate_default_likelihood(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;

    double likelihood = 0.0;

    constexpr int nBins = 44;

    for (const auto detector : {ND, FDI, FDII}) {
      using map_t   = Eigen::Map<const Eigen::Array<double, nBins, 1>>;
      using array_t = Eigen::Array<double, nBins, 1>;

      // Get all spectrum components as Eigen::Map
      // map_t acc(m_Accidental.get_spectrum(detector).data(), nBins);
      // map_t li(m_Lithium.get_spectrum(detector).data(), nBins);
      // map_t fastN(m_FastN.get_spectrum(detector).data(), nBins);
      // map_t dnc(m_DNC.get_spectrum(detector).data(), nBins);
      map_t reactor(m_Reactor.get_spectrum(detector).data(), nBins);

      // Add all background components to the full background contribution
      const array_t bkg = array_t::Ones();  // acc + li + fastN + dnc;

      // Get the measurement data as Eigen::Map
      map_t data(get_measurement_data(detector).data(), nBins);

      // Get the MC normalization parameter
      // const double mcNorm = parameter[params::index(detector, params::dc::Detector::MCNorm)];

      // Calculate the full spectrum prediction
      array_t prediction = (bkg + (1.0 * reactor));

      print_array(data, "data");
      print_array({prediction.data(), 44}, "prediction");

      // Calculate Poisson Likelihood
      likelihood += -2.0 * ((data + 1) * prediction.log() - prediction).sum();
      std::cout << params::dc::get_detector_name(detector) << '\t' << likelihood << '\n';

      // Calculate the off-off component of the likelihood
      // if (detector == ND || detector == FDII) {
      //   likelihood += calculate_off_off_likelihood(bkg, detector);
      //   std::cout << params::dc::get_detector_name(detector) << "_off_off" << '\t' << likelihood << '\n';
      // }
    }

    // Return the likelihood parameter if it is finite, otherwise return a large number. This is to prevent the minimizer from crashing.
    return std::isfinite(likelihood) ? likelihood : 1.0e25;
  }
}  // namespace ana::dc
