#pragma once

#include "../Definitions.h"
#include "../ParameterWrapper.h"
#include "../SpectrumBase.h"

namespace ana::dc {

  class DNCBackground : public SpectrumBase {
   public:
    explicit DNCBackground(std::shared_ptr<io::Options> options)
      : SpectrumBase(std::move(options)) {}

    ~DNCBackground() override = default;

    bool check_and_recalculate_spectra(const ParameterWrapper& parameter) override;

    [[nodiscard]] const Eigen::Array<double, 44, 1>& get_spectrum(params::dc::DetectorType type) const noexcept override;

   private:
    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;

    std::unordered_map<params::dc::DetectorType, return_t> m_Cache;
    std::unordered_map<params::dc::DetectorType, return_t> m_SpectrumTemplate_Gd;
    std::unordered_map<params::dc::DetectorType, return_t> m_SpectrumTemplate_Hy;
  };

}  // namespace ana::dc
