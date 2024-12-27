#include "AccidentalBackground.h"

#include <algorithm>
#include "Eigen/Core"
#include "Eigen/Eigenvalues"

namespace ana::dc {

  template <typename T1, typename T2>
  inline void scale_and_clip_vector(const T1& v, T2& result,  double scale) noexcept {
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

  bool AccidentalBackground::check_and_recalculate(const ParameterWrapper &parameter) {
    const bool recalculate = check_parameters(parameter);
    if (recalculate) {
      recalculate_spectra(parameter);
    }
    return recalculate;
  }


}

