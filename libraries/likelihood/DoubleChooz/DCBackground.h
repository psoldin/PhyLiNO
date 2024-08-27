#pragma once

#include "../Definitions.h"
#include "AccidentalBackground.h"
#include "LithiumBackground.h"
#include "SpectrumBase.h"

namespace ana::dc {

  class FastNBkackground;
  class DNCBackground;

  class DCBackground : public SpectrumBase<DCBackground> {
    friend class SpectrumBase<DCBackground>;

   public:
    explicit DCBackground(std::shared_ptr<io::Options> options)
      : SpectrumBase<DCBackground>(options)
      , m_Accidental(options)
      , m_Lithium(options) {}

    ~DCBackground() = default;

    void check_and_recalculate_spectrum(const ParameterWrapper& parameter) {
      m_Accidental.check_and_recalculate_spectrum(parameter);
      m_Lithium.check_and_recalculate_spectrum(parameter);
    }

    [[nodiscard]] Eigen::Array<double, 44, 1> return_spectrum(params::dc::DetectorType type) const noexcept {
      const auto& acc = m_Accidental.return_spectrum(type);
      const auto& lit = m_Lithium.return_spectrum(type);

      return acc + lit;
    }

   protected:
    AccidentalBackground m_Accidental;
    LithiumBackground    m_Lithium;
    // FastNBkackground m_FastN;
    // DNCBackground m_DNC;

    return_t m_AccidentalCache;
    return_t m_LithiumCache;
    return_t m_FastNCache;
    return_t m_DNCCache;

    [[nodiscard]] Eigen::Array<double, 44, 1> return_spectrum(const ParameterWrapper& parameter, params::dc::DetectorType type) {
      return {};
    }
  };
}  // namespace ana::dc
