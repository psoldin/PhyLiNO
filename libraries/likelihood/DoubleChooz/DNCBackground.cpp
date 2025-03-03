#include "DNCBackground.h"

namespace ana::dc {

  inline bool parameter_changed(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;

    bool has_changed = false;
    for (auto detector : {ND, FDI, FDII}) {
      has_changed |= parameter.check_parameter_changed(params::index(detector, BkgRDNCGd));
      has_changed |= parameter.check_parameter_changed(params::index(detector, BkgRDNCHy));
    }

    return has_changed;
  }

  DNCBackground::DNCBackground(std::shared_ptr<io::Options> options)
    : SpectrumBase(std::move(options)) {
    using enum params::dc::DetectorType;
      std::array<double, 44> null_shape{};
      std::ranges::fill(null_shape, 0.0);
    for (auto detector : {ND, FDI, FDII}) {
      m_SpectrumTemplate_Gd[detector] = null_shape;
      m_SpectrumTemplate_Hy[detector] = null_shape;
      m_Cache[detector] = null_shape;
    }
  }

  bool DNCBackground::check_and_recalculate(const ParameterWrapper& parameter) {
    bool has_changed = parameter_changed(parameter);
    if (has_changed) {
      recalculate_spectra(parameter);
    }
    return has_changed;
  }

  void DNCBackground::recalculate_spectra(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;

    for (auto detector : {ND, FDII}) {
      double gd_rate = parameter[params::index(detector, BkgRDNCGd)];
      double hy_rate = parameter[params::index(detector, BkgRDNCHy)];

      const auto& gd_shape = m_SpectrumTemplate_Gd[detector];
      const auto& hy_shape = m_SpectrumTemplate_Hy[detector];

      double lifetime = m_Options->double_chooz().dataBase().on_lifetime(detector);

      auto& result = m_Cache[detector];
      for (int i = 0; i < 44; ++i) {
        result[i] = std::max(lifetime * ((gd_rate * gd_shape[i]) + (hy_rate * hy_shape[i])), 0.0);
      }
    }
  }

  std::span<const double> DNCBackground::get_spectrum(params::dc::DetectorType type) const noexcept {
    return m_Cache.at(type);
  }
}  // namespace ana::dc
