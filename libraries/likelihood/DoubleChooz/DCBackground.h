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

    bool check_and_recalculate_spectra(const ParameterWrapper& parameter) override {
      bool has_changed = false;
      has_changed |= m_Accidental.check_and_recalculate_spectra(parameter);
      has_changed |= m_Lithium.check_and_recalculate_spectra(parameter);
      has_changed |= m_FastN.check_and_recalculate_spectra(parameter);

      if (has_changed) {
        using enum params::dc::DetectorType;

        for (auto detector : {ND, FDI, FDII}) {
          m_Cache[detector] = m_Accidental.get_spectrum(detector)
                              + m_Lithium.get_spectrum(detector)
                              + m_FastN.get_spectrum(detector);
        }
      }
      return has_changed;
    }

    [[nodiscard]] const Eigen::Array<double, 44, 1>& get_spectrum(params::dc::DetectorType type) const noexcept override {
      return m_Cache.at(type);
    }

   protected:
    AccidentalBackground m_Accidental;
    LithiumBackground    m_Lithium;
    FastNBackground      m_FastN;
    // DNCBackground m_DNC;

    std::unordered_map<params::dc::DetectorType, return_t> m_Cache;
  };
}  // namespace ana::dc
