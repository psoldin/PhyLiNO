#pragma once

#include "../Definitions.h"
#include "../SpectrumBase.h"
#include "Options.h"

namespace ana::dc {

  class LithiumBackground : public SpectrumBase {
   public:
    explicit LithiumBackground(std::shared_ptr<io::Options> options);

    ~LithiumBackground() override = default;

    bool check_and_recalculate(const ParameterWrapper& parameter) override;

    [[nodiscard]] std::span<const double> get_spectrum(params::dc::DetectorType detector) const noexcept override {
      return m_LiSpectrum.at(detector);
    }

    [[nodiscard]] std::span<const double> get_background_template(params::dc::DetectorType type) const {
      return m_BackgroundTemplate.at(type);
    }

   private:
    using array_t = std::array<double, 44>;

    template <typename T>
    using map_t = std::unordered_map<params::dc::DetectorType, T>;

    map_t<array_t> m_BackgroundTemplate;
    map_t<array_t> m_LiSpectrum;

    map_t<std::shared_ptr<Eigen::MatrixXd>> m_CovMatrix;

    void recalculate_spectra(const ParameterWrapper& parameter);

    void fill_data(params::dc::DetectorType);
  };

}  // namespace ana::dc
