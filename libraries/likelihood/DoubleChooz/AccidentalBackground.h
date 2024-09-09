#pragma once

#include "../Definitions.h"
#include "ParameterWrapper.h"
#include "SpectrumBase.h"

namespace ana::dc {

  class AccidentalBackground : public SpectrumBase {
   public:
    explicit AccidentalBackground(std::shared_ptr<io::Options> options)
      : SpectrumBase(std::move(options)) {}

    ~AccidentalBackground() override = default;

    void check_and_recalculate_spectra(const ParameterWrapper& parameter) override;

    [[nodiscard]] const return_t& get_spectrum(params::dc::DetectorType type) const noexcept override {
      return m_Cache.at(type);
    }

   private:
    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;

    std::unordered_map<params::dc::DetectorType, return_t>                    m_Cache;
    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 38, 1>> m_SpectrumTemplate;
  };
}  // namespace ana::dc
