#pragma once

#include "../Definitions.h"
#include "Options.h"
#include "../SpectrumBase.h"

namespace ana::dc {

  class LithiumBackground : public SpectrumBase {
   public:
    explicit LithiumBackground(std::shared_ptr<io::Options> options);

    ~LithiumBackground() override = default;

    [[nodiscard]] bool check_and_recalculate(const ParameterWrapper& parameter) override;

    [[nodiscard]] std::span<const double> get_spectrum(params::dc::DetectorType detector) const noexcept override {
      return m_LiSpectrum.at(detector);
    }

    [[nodiscard]] std::span<const double> get_background_template(params::dc::DetectorType type) const {
      return m_BackgroundTemplate.at(type);
    }

  private:
    using map_t = std::unordered_map<params::dc::DetectorType, std::array<double, 44>>;

    map_t m_BackgroundTemplate;
    map_t m_LiSpectrum;

    void recalculate_spectra(const ParameterWrapper& parameter);

    void fill_data();
  };

}  // namespace ana::dc
