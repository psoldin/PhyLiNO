#pragma once

#include "SpectrumBase.h"
#include "../Definitions.h"

namespace ana::dc {

  class AccidentalBackground : public SpectrumBase<AccidentalBackground> {
    friend class SpectrumBase<AccidentalBackground>;

   public:
    explicit AccidentalBackground(std::shared_ptr<io::Options> options)
      : SpectrumBase<AccidentalBackground>(std::move(options)) {}

    ~AccidentalBackground() = default;

    void check_and_recalculate_spectrum(const ParameterWrapper& parameter);

    [[nodiscard]] Eigen::Array<double, 44, 1> return_spectrum(params::dc::DetectorType type) const noexcept {
      return {};
    }

   private:
    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;

    std::unordered_map<params::dc::DetectorType, return_t> m_Cache;
    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 38, 1>> m_SpectrumTemplate;
  };
}  // namespace ana::dc
