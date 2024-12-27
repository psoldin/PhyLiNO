#pragma once

#include "../Definitions.h"
#include "Options.h"
#include "../SpectrumBase.h"

namespace ana::dc {

  class LithiumBackground : public SpectrumBase {
   public:
    explicit LithiumBackground(std::shared_ptr<io::Options> options)
      : SpectrumBase(std::move(options)) {}

    ~LithiumBackground() override = default;

    [[nodiscard]] bool check_and_recalculate(const ParameterWrapper& parameter) override;

    [[nodiscard]] std::span<const double> get_spectrum(params::dc::DetectorType detector) const noexcept override {
      return m_Spectra.at(detector);
    }

  private:
    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 44, 1>> m_Spectra;

    void recalculate_spectra(const ParameterWrapper& parameter);
  };

}  // namespace ana::dc
