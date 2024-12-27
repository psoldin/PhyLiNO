#pragma once

#include "../Definitions.h"
#include "../SpectrumBase.h"

namespace ana::dc {

  class FastNBackground : public SpectrumBase {
   public:
    explicit FastNBackground(std::shared_ptr<io::Options> options)
      : SpectrumBase(std::move(options)) {}

    ~FastNBackground() override = default;

    bool check_and_recalculate(const ParameterWrapper& parameter) override {
      const bool recalculate = check_parameters(parameter);
      if (!check_parameters(parameter)) {
        recalculate_spectra(parameter);
      }
      return recalculate;
    }

    [[nodiscard]] std::span<const double> get_spectrum(params::dc::DetectorType detector) const noexcept override {
      return m_Spectra.at(detector);
    }

   private:
    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 44, 1>> m_Spectra;

    void recalculate_spectra(const ParameterWrapper& parameter) {
      // Recalculate the lithium background spectrum
    }

    [[nodiscard]] static bool check_parameters(const ParameterWrapper& parameter) noexcept;
  };
}  // namespace ana::dc
