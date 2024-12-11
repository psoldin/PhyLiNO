#pragma once

#include "../Definitions.h"
#include "../ParameterWrapper.h"
#include "SpectrumBase.h"

namespace ana::dc {

  class AccidentalBackground : public SpectrumBase {
   public:
    explicit AccidentalBackground(std::shared_ptr<io::Options> options)
      : SpectrumBase(std::move(options)) {}

    ~AccidentalBackground() override = default;

    bool check_and_recalculate_spectra(const ParameterWrapper& parameter) override;

    [[nodiscard]] std::span<const double> get_spectrum(params::dc::DetectorType type) const noexcept override {
      return m_Cache.at(type);
    }

   private:
    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;

    template <typename T>
    using uo_map = std::unordered_map<params::dc::DetectorType, T>;

    uo_map<return_t<44>>                m_Cache;
    uo_map<Eigen::Array<double, 38, 1>> m_SpectrumTemplate;
  };
}  // namespace ana::dc
