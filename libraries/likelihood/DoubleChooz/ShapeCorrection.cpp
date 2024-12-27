#include "ShapeCorrection.h"
#include "Calculate_Spectrum.h"
#include "Oscillator.h"

namespace ana::dc {

  [[nodiscard]] bool parameter_changed(const ParameterWrapper& parameter) noexcept {

      using enum params::dc::DetectorType;
      using namespace params::dc;

      bool fd1_changed = parameter.check_parameter_changed(params::index(FDI, Detector::NuShape01),
                                                           params::index(FDI, Detector::NuShape43));

      bool fd2_changed = parameter.check_parameter_changed(params::index(FDII, Detector::NuShape01),
                                                           params::index(FDII, Detector::NuShape43));

      bool nd_changed = parameter.check_parameter_changed(params::index(ND, Detector::NuShape01),
                                                          params::index(ND, Detector::NuShape43));

      bool parameter_changed = fd1_changed | fd2_changed | nd_changed;

      return parameter_changed;
  }

  bool ShapeCorrection::check_and_recalculate(const ParameterWrapper &parameter) noexcept {

    const bool previous_step = m_Oscillator->check_and_recalculate(parameter);
    const bool this_step = parameter_changed(parameter);
    const bool recalculate = previous_step | this_step;

    if (recalculate) {
      recalculate_spectra(parameter);
    }

    return recalculate;
  }

  void ShapeCorrection::recalculate_spectra(const ParameterWrapper& parameter) noexcept {

    using enum params::dc::DetectorType;
    using namespace params::dc;
    //
    // for (auto detector : {ND, FDI, FDII}) {
    //   const auto& shape = m_Oscillator->get_spectrum(detector);
    //   auto shape_parameter = parameter.sub_range(params::index(detector, Detector::NuShape01),
    //                                              params::index(detector, Detector::NuShape43) + 1);
    //
    //   const auto& covMatrix = m_Options->dataBase().covariance_matrix(detector, io::SpectrumType::Reactor);
    //   auto& result = m_Cache[detector];
    //
    //   calculate_spectrum<43>(1.0,
    //                          shape,
    //                          shape_parameter,
    //                          covMatrix.block<43, 43>(0, 0),
    //                          result);
    // }
  }

}

