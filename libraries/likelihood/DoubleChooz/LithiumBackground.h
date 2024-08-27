#pragma once

#include "Options.h"
#include "SpectrumBase.h"
#include "../Definitions.h"

namespace ana::dc {

  class LithiumBackground : public SpectrumBase<LithiumBackground> {
   public:
    explicit LithiumBackground(std::shared_ptr<io::Options> options)
      : SpectrumBase<LithiumBackground>(std::move(options)) {}

    ~LithiumBackground() = default;

    void check_and_recalculate_spectrum(const ParameterWrapper& parameter);

    [[nodiscard]] const return_t& return_spectrum(params::dc::DetectorType type) const noexcept {
      return m_Cache.at(type);
    }

  private:
    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;

    std::unordered_map<params::dc::DetectorType, return_t> m_Cache;
    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 38, 1>> m_SpectrumTemplate;
  };

}  // namespace ana::dc
