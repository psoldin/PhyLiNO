#pragma once

#include "../Definitions.h"
#include "../SpectrumBase.h"

namespace ana::dc {

  class FastNBackground : public SpectrumBase {
   public:
    explicit FastNBackground(std::shared_ptr<io::Options> options)
      : SpectrumBase(std::move(options)) {}

    ~FastNBackground() override = default;

    [[nodiscard]] std::span<const double> get_spectrum(params::dc::DetectorType detector) const noexcept override {
      return m_Spectra.at(detector);
    }

    [[nodiscard]] bool check_and_recalculate(const ParameterWrapper &parameter) override;

   private:
    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 44, 1>> m_Spectra;

    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;
  };
}  // namespace ana::dc
