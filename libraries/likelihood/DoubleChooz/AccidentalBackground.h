#pragma once

#include "../SpectrumBase.h"

namespace ana::dc {

  class AccidentalBackground : public SpectrumBase {
  public:
    explicit AccidentalBackground(std::shared_ptr<io::Options> options)
      : SpectrumBase(std::move(options)) { }

    ~AccidentalBackground() override = default;

    bool check_and_recalculate_spectra(const ParameterWrapper& parameter) override;

    [[nodiscard]] std::span<const double> get_spectrum(params::dc::DetectorType detector) const override {
      return m_Spectra.at(detector);
    }

  private:
    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 44, 1>> m_Spectra;

    void recalculate_spectra(const ParameterWrapper& parameter) {
      // Recalculate the accidental background spectrum
    }

  };

}