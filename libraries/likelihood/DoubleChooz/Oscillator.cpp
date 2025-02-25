#include "Oscillator.h"
#include "ThreeFlavorOscillation.h"

namespace ana::dc {

  inline std::vector<int> get_indices(std::span<const double> evis) {
    unsigned int index = 0;
    double       cond  = 0.25;

    std::vector<int> indices;
    int              i = 0;
    for (double evi : evis) {
      if (evi >= cond) {
        indices.push_back(i);
        index += 1;
        cond = static_cast<double>(index + 1) * 0.25;
      }
      ++i;
    }
    return indices;
  }

  Oscillator::Oscillator(std::shared_ptr<io::Options> options)
    : SpectrumBase(std::move(options)) {
    using enum params::dc::DetectorType;

    for (const auto detector : {ND, FDI, FDII}) {
      const auto& reactorData = m_Options->double_chooz().dataBase().reactor_data(detector);
      add_reactor_data(reactorData, detector);
    }

    //
    // if (m_Options->inputOptions().use_reactor_split()) {
    //   for (auto detector : {ND, FDII}) {
    //     const auto DB1 = cast_to_B1_split(detector);
    //     const auto& reactorData_B1 = m_Options->dataBase().reactor_data(DB1);
    //     add_reactor_data(reactorData_B1, DB1);
    //
    //     const auto DB2 = cast_to_B2_split(detector);
    //     const auto& reactorData_B2 = m_Options->dataBase().reactor_data(DB2);
    //     add_reactor_data(reactorData_B2, DB1);
    //   }
    // }
  }

  void Oscillator::add_reactor_data(const io::ReactorData& reactorData, params::dc::DetectorType type) {
    // Get the L over E data
    span_t LoverE = reactorData.LoverE();

    // Get the visual energy data
    span_t evis = reactorData.evis();

    // Get the scaling data
    span_t scaling = reactorData.scaling();

    // Get the target bin indices
    const std::vector<int> indices = get_indices(evis);

    for (unsigned int i = 1; i < indices.size(); ++i) {
      m_CalculationData.emplace_back(std::span<const double>(&LoverE[indices[i - 1]], indices[i] - indices[i - 1]),
                                    std::span<const double>(&scaling[indices[i - 1]], indices[i] - indices[i - 1]),
                                    i,
                                    type);
    }
  }

  inline bool check_parameter(const ParameterWrapper& parameter) noexcept {
    using enum params::General;

    bool recalculate = parameter.check_parameter_changed(SinSqT13);
    recalculate |= parameter.check_parameter_changed(DeltaM31);
    recalculate |= parameter.check_parameter_changed(SinSqT12);
    recalculate |= parameter.check_parameter_changed(DeltaM21);
    recalculate |= parameter.check_parameter_changed(SinSqT14);
    recalculate |= parameter.check_parameter_changed(DeltaM41);

    return recalculate;
  }

  void Oscillator::recalculate_spectra(const ParameterWrapper& parameter) noexcept {
    perform_cpu_oscillation(parameter);
  }

  bool Oscillator::check_and_recalculate(const ParameterWrapper &parameter) noexcept {
    const bool recalculate = check_parameter(parameter);

    if (recalculate) {
      recalculate_spectra(parameter);
    }

    return recalculate;
  }

  void Oscillator::perform_cpu_oscillation(const ParameterWrapper& parameter) noexcept {
    const std::size_t N = m_CalculationData.size();

    for (auto& [_, spectra] : m_Cache) {
      std::ranges::fill(spectra, 0.0);
    }

    const ThreeFlavorOscillation osci(parameter[params::General::SinSqT13],
                                      parameter[params::General::DeltaM31],
                                      parameter[params::General::SinSqT12],
                                      parameter[params::General::DeltaM21]);

    // #pragma omp parallel for
    for (std::size_t i = 0UL; i < N; ++i) {
      const auto& data                    = m_CalculationData[i];
      m_Cache[data.type][data.target_bin] = osci(data);
    }
  }

}  // namespace ana::dc
