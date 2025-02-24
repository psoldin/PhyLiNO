#include "LithiumBackground.h"
#include "Calculate_Spectrum.h"
#include <DoubleChooz/Constants.h>

// STL includes
#include <numeric>

// ROOT includes
#include <TH1D.h>

namespace ana::dc {

  [[nodiscard]] inline bool check_parameters(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using namespace params;
    using namespace params::dc;

    bool recalculate = parameter.check_parameter_changed(General::LiShape01, General::LiShape38);

    recalculate |= parameter.check_parameter_changed(index(ND, BkgRLi));
    recalculate |= parameter.check_parameter_changed(index(FDI, BkgRLi));
    recalculate |= parameter.check_parameter_changed(index(FDII, BkgRLi));

    return recalculate;
  }

  LithiumBackground::LithiumBackground(std::shared_ptr<io::Options> options)
    : SpectrumBase(std::move(options)) {
    fill_data();
  }

  bool LithiumBackground::check_and_recalculate(const ParameterWrapper &parameter) {
    bool has_changed = check_parameters(parameter);
    if (has_changed) {
      recalculate_spectra(parameter);
    }
    return has_changed;
  }

  void LithiumBackground::recalculate_spectra(const ParameterWrapper& parameter) {
    // using enum params::dc::DetectorType;
    // using enum params::dc::Detector;
    // using namespace params;
    //
    // for (auto detector : {ND, FDI, FDII}) {
    //   double      rate            = parameter[params::index(detector, BkgRLi)];
    //   const auto& shape           = m_SpectrumTemplate[detector];
    //   auto        shape_parameter = parameter.sub_range(General::LiShape01, General::LiShape38 + 1);
    //   const auto& covMatrix       = m_Options->dataBase().covariance_matrix(detector, io::SpectrumType::Lithium);
    //   auto&       result          = m_Cache[detector];
    //
    //   calculate_spectrum<38>(rate,
    //                          shape,
    //                          shape_parameter,
    //                          covMatrix.block<38, 38>(0, 0),
    //                          result);
    // }
  }

  void LithiumBackground::fill_data() {
    auto acc_data = m_Options->double_chooz().dataBase().background_data(params::dc::BackgroundType::lithium);

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
      std::cout << params::dc::get_detector_name(detector) << '\n';
      const double lifeTime = m_Options->double_chooz().dataBase().on_lifetime(detector);

      std::array<double, 44> background_spectrum;

      const double scaling_factor = lifeTime / sum;

      for (int i = 0; i < 44; ++i) {
        background_spectrum[i] = scaling_factor * background_template[i];
      }

      m_BackgroundTemplate[detector] = background_spectrum;
    }
  }
}  // namespace ana::dc
