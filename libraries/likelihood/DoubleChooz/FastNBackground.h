#pragma once

#include "../Definitions.h"
#include "../SpectrumBase.h"

namespace ana::dc {

  class FastNBackground : public SpectrumBase {
   public:
    explicit FastNBackground(std::shared_ptr<io::Options> options);

    ~FastNBackground() override = default;

    [[nodiscard]] bool check_and_recalculate(const ParameterWrapper& parameter) override;

    [[nodiscard]] std::span<const double> get_spectrum(params::dc::DetectorType detector) const override {
      return m_FastNSpectrum.at(detector);
    }

    [[nodiscard]] std::span<const double> get_background_template(params::dc::DetectorType detector) const {
      return m_BackgroundTemplate.at(detector);
    }

   private:
    template <typename T>
    using map_t = std::unordered_map<params::dc::DetectorType, T>;

    map_t<std::array<double, 44>>           m_BackgroundTemplate;
    map_t<std::array<double, 44>>           m_FastNSpectrum;
    map_t<std::shared_ptr<Eigen::MatrixXd>> m_CovMatrix;

    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;

    void fill_data();
  };
}  // namespace ana::dc
