#pragma once

#include "../Definitions.h"
#include "AccidentalBackground.h"
#include "FastNBackground.h"
#include "LithiumBackground.h"
#include "SpectrumBase.h"

namespace ana::dc {

  class DCBackground : public SpectrumBase {
   public:
    explicit DCBackground(std::shared_ptr<io::Options> options);

    ~DCBackground() override = default;

    bool check_and_recalculate_spectra(const ParameterWrapper& parameter) override;

    [[nodiscard]] const Eigen::Array<double, 44, 1>& get_spectrum(params::dc::DetectorType type) const noexcept override;

   protected:
    AccidentalBackground m_Accidental;
    LithiumBackground    m_Lithium;
    FastNBackground      m_FastN;
    // DNCBackground m_DNC;

    std::unordered_map<params::dc::DetectorType, return_t> m_Cache;
  };
}  // namespace ana::dc
