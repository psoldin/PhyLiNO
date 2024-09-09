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
    : m_Options(std::move(options)) {

    using enum params::dc::DetectorType;

    for (auto detector : {ND, FDI, FDII}) {
      const auto& reactorData = m_Options->dataBase().reactor_data(detector);
      add_reactor_data(reactorData, detector);
    }

  }

  void Oscillator::add_reactor_data(const io::ReactorData &reactorData, params::dc::DetectorType type) {

    // Get the L over E data
    span_t LoverE = reactorData.LoverE();

    // Get the visual energy data
    span_t evis = reactorData.evis();

    // Get the scaling data
    span_t scaling = reactorData.scaling();

    // Get the target bin indices
    const std::vector<int> indices = get_indices(evis);

    for (int i = 1; i < indices.size(); ++i) {
      // Add the calculation data
      m_CalculationData.emplace_back(LoverE.subspan(indices[i - 1], indices[i] - indices[i - 1]),
                                     scaling.subspan(indices[i - 1], indices[i] - indices[i - 1]),
                                     i, type);
    }
  }

  #pragma omp declare simd
  template <typename T>
  inline auto pow_2(T&& x) noexcept { return x * x; }

  void Oscillator::recalculate_spectra(const ParameterWrapper& parameter, std::vector<Eigen::Array<double, 80, 1>>& spectra) const noexcept {

    const std::size_t N = m_CalculationData.size();

    for (auto& spectrum : spectra)
      std::fill(spectrum.begin(), spectrum.end(), 0.0);

    const ThreeFlavorOscillation osci(parameter[params::General::SinSqT13],
                                      parameter[params::General::DeltaM31],
                                      parameter[params::General::SinSqT12],
                                      parameter[params::General::DeltaM21]);

    // #pragma omp parallel for
    for (std::size_t i = 0UL; i < N; ++i) {
      const auto& data = m_CalculationData[i];
      spectra[params::get_index(data.type)][data.target_bin] = osci(data);
    }
  }

}  // namespace ana
