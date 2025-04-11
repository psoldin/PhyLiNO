#include "ShapeCorrection.h"

#include <numeric>

#include "Calculate_Spectrum.h"
#include "Oscillator.h"

namespace ana::dc {

  [[nodiscard]] bool parameter_changed(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using namespace params;
    using namespace params::dc;

    const bool fd1_changed = parameter.check_parameter_changed(index(FDI, NuShape01),
                                                               index(FDI, NuShape43));

    const bool fd2_changed = parameter.check_parameter_changed(index(FDII, NuShape01),
                                                               index(FDII, NuShape43));

    const bool nd_changed = parameter.check_parameter_changed(index(ND, NuShape01),
                                                              index(ND, NuShape43));

    const bool parameter_changed = fd1_changed | fd2_changed | nd_changed;

    return parameter_changed;
  }

  ShapeCorrection::ShapeCorrection(std::shared_ptr<io::Options> options, std::shared_ptr<Oscillator> oscillator)
    : SpectrumBase(std::move(options))
    , m_Oscillator(std::move(oscillator)) {
    using enum params::dc::DetectorType;

    const auto& db = m_Options->double_chooz().dataBase();

    for (auto detector : {ND, FDI, FDII}) {
      const auto& cov       = db.covariance_matrix(detector, params::dc::SpectrumType::reactor);
      m_CovMatrix[detector] = cov;
    }
  }

  bool ShapeCorrection::check_and_recalculate(const ParameterWrapper& parameter) noexcept {
    const bool previous_step = m_Oscillator->check_and_recalculate(parameter);
    const bool this_step     = parameter_changed(parameter);
    const bool recalculate   = previous_step | this_step;

    if (recalculate) {
      recalculate_spectra(parameter);
    }

    return recalculate;
  }

  void ShapeCorrection::recalculate_spectra(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using namespace params::dc;

    // The reactor spectrum does not need an explicit rate
    // This is done later with other parameters, this here is just a placeholder for the function call
    const double rate = 1.0;

    for (const auto detector : {ND, FDI, FDII}) {
      const std::span<const double> oscillated_spectrum = m_Oscillator->get_spectrum(detector);

      const auto shape_parameter = parameter.sub_range(params::index(detector, NuShape01),
                                                       params::index(detector, NuShape43) + 1);

      assert(m_CovMatrix[detector] != nullptr);

      const Eigen::MatrixXd& covMatrix = *m_CovMatrix[detector];

      std::array<double, 80>& result = m_Cache[detector];

      calculate_spectrum(rate,
                         oscillated_spectrum,
                         shape_parameter,
                         covMatrix,
                         result);
    }
  }

}  // namespace ana::dc
