#include "FastNBackground.h"
#include "Calculate_Spectrum.h"
#include "Parameter.h"
#include <DoubleChooz/Constants.h>

// STL includes
#include <numeric>

// ROOT includes
#include <TH1D.h>

namespace ana::dc {

  inline bool parameter_changed(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;
    using namespace params;

    bool has_changed = false;

    for (auto detector : {ND, FDI, FDII}) {
      has_changed |= parameter.check_parameter_changed(index(detector, FNSMShape01), index(detector, FNSMShape44) + 1);
      has_changed |= parameter.check_parameter_changed(index(detector, BkgRFNSM));
    }

    return has_changed;
  }

  FastNBackground::FastNBackground(std::shared_ptr<io::Options> options)
    : SpectrumBase(std::move(options)) {
    fill_data();
  }

  bool FastNBackground::check_and_recalculate(const ParameterWrapper &parameter) {
    bool has_changed = parameter_changed(parameter);
    if (has_changed) {
      recalculate_spectra(parameter);
    }
    return has_changed;
  }

  void FastNBackground::recalculate_spectra(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;
    using namespace params;
    //
    // for (auto detector : {ND, FDI, FDII}) {
    //   double      rate            = parameter[params::index(detector, BkgRFNSM)];
    //   const auto& shape           = m_SpectrumTemplate[detector];
    //   auto        shape_parameter = parameter.sub_range(index(detector, FNSMShape01), index(detector, FNSMShape44) + 1);
    //   const auto& covMatrix       = m_Options->dataBase().covariance_matrix(detector, io::SpectrumType::FastN);
    //   auto&       result          = m_Cache[detector];
    //
    //   calculate_spectrum<44>(rate,
    //                          shape,
    //                          shape_parameter,
    //                          covMatrix,
    //                          result);
    // }
  }

  void FastNBackground::fill_data() {
    auto acc_data = m_Options->double_chooz().dataBase().background_data(params::dc::BackgroundType::fastN);

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

      for (int i = 0; i < 44; ++i) {
        background_spectrum[i] = (lifeTime / sum) * background_template[i];
      }

      m_BackgroundTemplate[detector] = background_spectrum;
    }
  }

}  // namespace ana::dc
