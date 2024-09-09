#pragma once

#include "../Definitions.h"
#include "AccidentalBackground.h"
#include "FastNBackground.h"
#include "LithiumBackground.h"
#include "SpectrumBase.h"

namespace ana::dc {

  class DCBackground : public SpectrumBase {
   public:
    explicit DCBackground(std::shared_ptr<io::Options> options)
      : SpectrumBase(options)
      , m_Accidental(options)
      , m_Lithium(options)
      , m_FastN(options) {}

    ~DCBackground() override = default;

    void check_and_recalculate_spectra(const ParameterWrapper& parameter) override {
      m_Accidental.check_and_recalculate_spectra(parameter);
      m_Lithium.check_and_recalculate_spectra(parameter);
      m_FastN.check_and_recalculate_spectra(parameter);
    }

    [[nodiscard]] const Eigen::Array<double, 44, 1>& get_spectrum(params::dc::DetectorType type) const noexcept override {
      const auto& acc   = m_Accidental.get_spectrum(type);
      const auto& lit   = m_Lithium.get_spectrum(type);
      const auto& fastN = m_FastN.get_spectrum(type);

      return acc + lit + fastN;
    }

   protected:
    AccidentalBackground m_Accidental;
    LithiumBackground    m_Lithium;
    FastNBackground      m_FastN;
    // DNCBackground m_DNC;

    std::unordered_map<params::dc::DetectorType, return_t> m_Cache;

    [[nodiscard]] Eigen::Array<double, 44, 1> return_spectrum(const ParameterWrapper& parameter, params::dc::DetectorType type) {
      return {};
    }
  };
}  // namespace ana::dc
