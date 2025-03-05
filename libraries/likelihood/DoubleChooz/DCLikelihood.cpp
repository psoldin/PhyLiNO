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
    throw std::runtime_error("Reading measurement data is not yet implemented");
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

    std::cout << "Setting starting parameters as set in config file for Asimov data set generation!\n";
    for (std::size_t i = 0; i < parameter.size(); ++i) {
      parameter[i] = pv[i].value();
    }

    std::cout << "Set SinSqT13 = 0.1 in the parameter array for Asimov data set generation!\n";
    parameter[params::SinSqT13] = 0.1;

    m_Parameter.reset_parameter(parameter.data());

    std::cout << "Calculate the spectrum components for Asimov data set generation!\n";
    for (auto* component : m_Components) {
      component->check_and_recalculate(m_Parameter);
    }

    using enum params::dc::DetectorType;

    constexpr int nBins = 44;

    for (auto detector : {ND, FDI, FDII}) {
      using map_t   = Eigen::Map<const Eigen::Array<double, nBins, 1>>;
      using array_t = Eigen::Array<double, nBins, 1>;

      // Get all spectrum components as Eigen::Map
      map_t acc(m_Accidental.get_spectrum(detector).data(), nBins);
      map_t li(m_Lithium.get_spectrum(detector).data(), nBins);
      map_t fastN(m_FastN.get_spectrum(detector).data(), nBins);
      map_t dnc(m_DNC.get_spectrum(detector).data(), nBins);
      map_t reactor(m_Reactor.get_spectrum(detector).data(), nBins);

      // Add all background components to the full background contribution
      const array_t bkg = acc + li + fastN + dnc;

      // Get the MC normalization parameter
      const double mcNorm = calculate_mcNorm(m_Parameter, detector);

      // Calculate the full spectrum prediction
      array_t prediction = (bkg + (mcNorm * reactor));

      std::array<double, 44> array{};
      std::ranges::copy(prediction, array.begin());
      m_MeasurementData[detector] = array;

      if (detector == ND || detector == FDII) {
        // Get the on and off lifetimes
        const double on_lifetime  = m_Options->double_chooz().dataBase().on_lifetime(detector);
        const double off_lifetime = m_Options->double_chooz().dataBase().off_lifetime(detector);

        std::array<double, 44> off_off_data{};

        const array_t off_off_bkg = (off_lifetime / on_lifetime) * bkg;

        std::ranges::copy(off_off_bkg, off_off_data.begin());
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
    m_Components = {&m_Accidental, &m_Lithium, &m_FastN, &m_DNC, &m_Reactor};
    initialize_measurement_data();
  }

  void DCLikelihood::setup_pulls() {
    const auto& input_parameters = m_Options->inputOptions().input_parameters();

    const auto& names       = input_parameters.names();
    const auto& fixed       = input_parameters.fixed();
    const auto& parameters  = input_parameters.parameters();
    const auto& constrained = input_parameters.constrained();

    for (std::size_t i = 0, end = m_Components.size(); i < end; ++i) {
      if (constrained[i]) {
        std::cout << "Setup pull for parameter " << std::setw(8) << i << ":\t" << names[i] << '\n';
        m_Pulls.emplace_back(i, parameters[i].value(), parameters[i].uncertainty());
      }
    }
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;
    for (int i = 0, end = NuShape43 - NuShape01 + 1; i < end; ++i) {
      m_ShapeCV.emplace_back(parameters[params::index(ND, i)].value(),
                             parameters[params::index(FDI, i)].value(),
                             parameters[params::index(FDII, i)].value());
    }
  }

  double DCLikelihood::calculate_pulls(const ParameterWrapper& parameter) const noexcept {
    double result = 0.0;
    for (const auto [idx, CV, sig] : m_Pulls) {
      result += pow_2((parameter[idx] - CV) / sig);
    }

    using span_t = std::span<const double>;

    span_t rawP = parameter.raw_parameters();

    using enum params::dc::DetectorType;
    using enum params::dc::Detector;

    constexpr size_t nShape = NuShape43 - NuShape01 + 1;

    span_t nd_shape  = rawP.subspan(params::index(ND, NuShape01), nShape);
    span_t fd1_shape = rawP.subspan(params::index(FDI, NuShape01), nShape);
    span_t fd2_shape = rawP.subspan(params::index(FDII, NuShape01), nShape);

    constexpr double scale = 1.0;

    for (std::size_t i = 0; i < nShape; ++i) {
      const auto [nd_CV, fd1_CV, fd2_CV] = m_ShapeCV[i];

      const double nd_result  = pow_2((nd_shape[i] - nd_CV) / scale);
      const double fd1_result = pow_2((fd1_shape[i] - fd1_CV) / scale);
      const double fd2_result = pow_2((fd2_shape[i] - fd2_CV) / scale);

      result += nd_result + fd1_result + fd2_result;
    }

    return result;
  }

  double DCLikelihood::calculate_likelihood(const double* parameter) {
    m_Parameter.reset_parameter(parameter);

    for (auto* component : m_Components) {
      component->check_and_recalculate(m_Parameter);
    }

    if (m_Options->inputOptions().double_chooz().reactor_split()) {
      return calculate_reactor_split_likelihood(m_Parameter);
    }
    return calculate_default_likelihood(m_Parameter);
  }

  double DCLikelihood::calculate_off_off_likelihood(const Eigen::Array<double, 44, 1>& bkg, params::dc::DetectorType detector) const {
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

  double DCLikelihood::calculate_mcNorm(const ParameterWrapper& parameter, params::dc::DetectorType type) const noexcept {
    using namespace params::dc;

    const auto [value, error] = m_Options->double_chooz().dataBase().mcNorm_central_values(type);

    const double norm = parameter[params::index(type, Detector::MCNorm)];

    const double result = value + error * norm;

    const double bugey4 = parameter[params::Bugey4];

    return result * bugey4;
  }

  double DCLikelihood::calculate_default_likelihood(const ParameterWrapper& parameter) const noexcept {
    using enum params::dc::DetectorType;

    double likelihood = 0.0;

    constexpr int nBins = 44;

    for (const auto detector : {ND, FDI, FDII}) {
      using map_t   = Eigen::Map<const Eigen::Array<double, nBins, 1>>;
      using array_t = Eigen::Array<double, nBins, 1>;

      // Get all spectrum components as Eigen::Map
      map_t acc(m_Accidental.get_spectrum(detector).data(), nBins);
      map_t li(m_Lithium.get_spectrum(detector).data(), nBins);
      map_t fastN(m_FastN.get_spectrum(detector).data(), nBins);
      map_t dnc(m_DNC.get_spectrum(detector).data(), nBins);
      map_t reactor(m_Reactor.get_spectrum(detector).data(), nBins);

      // Add all background components to the full background contribution
      const array_t bkg = acc + li + fastN + dnc;

      // Get the measurement data as Eigen::Map
      map_t data(get_measurement_data(detector).data(), nBins);

      // Get the MC normalization parameter
      const double mcNorm = calculate_mcNorm(parameter, detector);

      // Calculate the full spectrum prediction
      array_t prediction = (bkg + (mcNorm * reactor));

      // Calculate Poisson Likelihood
      likelihood += -2.0 * (data * prediction.log() - prediction).sum();

      // Calculate the off-off component of the likelihood
      // if (detector == ND || detector == FDII) {
      //   likelihood += calculate_off_off_likelihood(bkg, detector);
      //   std::cout << params::dc::get_detector_name(detector) << "_off_off" << '\t' << likelihood << '\n';
      // }
    }

    likelihood += calculate_pulls(parameter);

    // Return the likelihood parameter if it is finite, otherwise return a large number. This is to prevent the minimizer from crashing.
    return std::isfinite(likelihood) ? likelihood : 1.0e25;
  }
}  // namespace ana::dc
