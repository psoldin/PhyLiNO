#pragma once

#include "../Definitions.h"
#include "../SpectrumBase.h"

namespace ana::dc {

  class FastNBackground : public SpectrumBase {
   public:
    explicit FastNBackground(std::shared_ptr<io::Options> options)
      : SpectrumBase(std::move(options)) {}

    ~FastNBackground() = default;

    bool check_and_recalculate_spectra(const ParameterWrapper& parameter) override;

    [[nodiscard]] const return_t& get_spectrum(params::dc::DetectorType type) const noexcept override {
      return m_Cache.at(type);
    }

   private:
    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;

    std::unordered_map<params::dc::DetectorType, return_t>                    m_Cache;
    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 44, 1>> m_SpectrumTemplate;
  };
}  // namespace ana::dc
