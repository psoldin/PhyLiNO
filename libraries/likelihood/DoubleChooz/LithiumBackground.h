#pragma once

#include "../Definitions.h"
#include "Options.h"
#include "SpectrumBase.h"

namespace ana::dc {

  class LithiumBackground : public SpectrumBase {
   public:
    explicit LithiumBackground(std::shared_ptr<io::Options> options)
      : SpectrumBase(std::move(options)) {}

    ~LithiumBackground() override = default;

    bool check_and_recalculate_spectra(const ParameterWrapper& parameter) override;

    [[nodiscard]] const return_t& get_spectrum(params::dc::DetectorType type) const noexcept override {
      return m_Cache.at(type);
    }

   private:
    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;

    std::unordered_map<params::dc::DetectorType, return_t>                    m_Cache;
    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 38, 1>> m_SpectrumTemplate;
  };

}  // namespace ana::dc
