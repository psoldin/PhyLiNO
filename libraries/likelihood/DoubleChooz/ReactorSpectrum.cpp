#include "ReactorSpectrum.h"

namespace ana::dc {

  enum class CalculationType {
    Oscillation,
    EnergyCorrection,
    ShapeCorrection,
    None,
  };

  inline bool oscillation_parameter_changed(const ParameterWrapper& parameter) noexcept {
    using enum params::General;
    return parameter.check_parameter_changed(SinSqT13) || parameter.check_parameter_changed(DeltaM31);
  }

  inline bool energy_parameter_changed(const ParameterWrapper& parameter) noexcept {
    using enum params::General;
    using enum params::dc::DetectorType;
    bool has_changed = parameter.check_parameter_changed(EnergyA);
    for (auto detector : {ND, FDI, FDII}) {
      has_changed |= parameter.check_parameter_changed(params::index(detector, params::dc::Detector::EnergyB));
      has_changed |= parameter.check_parameter_changed(params::index(detector, params::dc::Detector::EnergyC));
    }
    return has_changed;
  }

  inline bool shape_parameter_changed(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;

    bool has_changed = false;
    for (auto detector : {ND, FDI, FDII}) {
      has_changed |= parameter.check_parameter_changed(params::index(detector, NuShape01),
                                                         params::index(detector, NuShape43) + 1);
    }

    return has_changed;
  }

  inline CalculationType check_parameter(const ana::dc::ParameterWrapper& parameter) noexcept {
    using namespace params::dc;
    using namespace params;
    using enum General;
    using enum CalculationType;

    if (oscillation_parameter_changed(parameter)) {
      return Oscillation;
    }

    if (energy_parameter_changed(parameter)) {
      return EnergyCorrection;
    }

    if (shape_parameter_changed(parameter)) {
      return ShapeCorrection;
    }

    return None;
  }

  bool ReactorSpectrum::check_and_recalculate_spectra(const ParameterWrapper& parameter) {

    auto calculation_type = check_parameter(parameter);

    switch (calculation_type) {
      case CalculationType::Oscillation:
        recalculate_oscillation_spectra(parameter);
        [[fallthrough]];
      case CalculationType::EnergyCorrection:
        recalculate_energy_correction(parameter);
        [[fallthrough]];
      case CalculationType::ShapeCorrection:
        recalculate_shape_correction(parameter);
        break;
      case CalculationType::None:
        break;
    }
    return true;
  }

}