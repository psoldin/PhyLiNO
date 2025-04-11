#include "FastNBackground.h"
#include <DoubleChooz/Constants.h>
#include "Calculate_Spectrum.h"
#include "Parameter.h"

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
    using enum params::dc::DetectorType;

    const auto& db = m_Options->double_chooz().dataBase();

    for (const auto detector : {ND, FDI, FDII}) {
      auto cov              = db.covariance_matrix(detector, params::dc::SpectrumType::fastN);
      m_CovMatrix[detector] = std::move(cov);
      m_FastNSpectrum[detector].fill(0.0);
      fill_data(detector);
    }
  }

  bool FastNBackground::check_and_recalculate(const ParameterWrapper& parameter) {
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

    for (const auto detector : {ND, FDI, FDII}) {
      using span_t = std::span<const double>;

      span_t background_template = get_background_template(detector);
      span_t shape_parameter     = parameter.sub_range(index(detector, FNSMShape01), index(detector, FNSMShape44) + 1);

      const double rate = parameter[index(detector, BkgRFNSM)];

      assert(m_CovMatrix[detector] != nullptr);

      const Eigen::MatrixXd& covMatrix = *m_CovMatrix[detector];

      std::array<double, 44>& result = m_FastNSpectrum[detector];

      calculate_spectrum(rate,
                         background_template,
                         shape_parameter,
                         covMatrix,
                         result);
    }
  }

  void FastNBackground::fill_data(params::dc::DetectorType type) {
    auto acc_data = m_Options->double_chooz().dataBase().background_data(type, params::dc::SpectrumType::fastN);

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
