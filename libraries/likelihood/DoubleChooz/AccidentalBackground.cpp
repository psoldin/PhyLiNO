// includes
#include "AccidentalBackground.h"

// STL includes
#include <algorithm>
#include <numeric>
#include <ranges>

// Eigen includes
#include "Eigen/Core"
#include "Eigen/Eigenvalues"

// ROOT includes
#include <TH1.h>

#include "DoubleChooz/Constants.h"

namespace ana::dc {

  template <typename T1, typename T2>
  inline void scale_and_clip_vector(const T1& v, T2& result, double scale) noexcept {
    std::transform(v.cbegin(),
                   v.cend(),
                   result.begin(),
                   [scale](double x) { return std::max(x * scale, 0.0); });
  }

  template <typename T1, typename T2>
  inline void add_vector1_to_vector2(const T1& v1, const T2& v2) noexcept {
    std::transform(v1.cbegin(),
                   v1.cend(),
                   v2.cbegin(),
                   v2.begin(),
                   std::plus<>());
  }

  /**
   * \brief Checks if any relevant parameters have changed and determines if spectra need to be recalculated.
   *
   * \param parameter The ParameterWrapper object containing the parameters to check.
   * \return True if any relevant parameters have changed, false otherwise.
   */
  [[nodiscard]] inline bool check_parameters(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using namespace params;
    using namespace params::dc;

    bool recalculate = false;
    for (auto detector : {ND, FDI, FDII}) {
      recalculate |= parameter.check_parameter_changed(index(detector, BkgRAcc));
      recalculate |= parameter.check_parameter_changed(index(detector, AccShape01), index(detector, AccShape38));
    }

    return recalculate;
  }

  AccidentalBackground::AccidentalBackground(std::shared_ptr<io::Options> options)
    : SpectrumBase(std::move(options)) {
    fill_data();
  }

  void AccidentalBackground::fill_data() {
    auto acc_data = m_Options->double_chooz().dataBase().background_data(params::dc::BackgroundType::accidental);

    const auto& binning = io::dc::Constants::EnergyBinXaxis;

    auto h = std::make_unique<TH1D>("h", "", binning.size() - 1, binning.data());

    for (auto E : acc_data) {
      h->Fill(E);
    }

    std::array<double, 44> background_template{};

    for (int i = 0; i < 44; ++i) {
      background_template[i] = h->GetBinContent(i + 1);
    }

    using enum params::dc::DetectorType;

    const double sum = std::accumulate(background_template.begin(), background_template.end(), 0.0);

    for (auto detector : {ND, FDI, FDII}) {
      const double lifeTime = m_Options->double_chooz().dataBase().on_lifetime(detector);

      std::array<double, 44> background_spectrum;

      std::cout << "LifeTime: " << lifeTime << '\n';
      std::cout << "Scaling: " << (lifeTime / sum) << '\n';

      for (int i = 0; i < 44; ++i) {
        background_spectrum[i] = (lifeTime / sum) * background_template[i];
      }

      m_BackgroundTemplate[detector] = background_spectrum;
    }
  }

  bool AccidentalBackground::check_and_recalculate(const ParameterWrapper& parameter) {
    const bool recalculate = check_parameters(parameter);
    if (recalculate) {
      recalculate_spectra(parameter);
    }
    return recalculate;
  }

}  // namespace ana::dc
