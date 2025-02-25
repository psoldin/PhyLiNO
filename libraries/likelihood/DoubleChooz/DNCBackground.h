#pragma once

#include <DoubleChooz/Constants.h>

#include "../Definitions.h"
#include "../ParameterWrapper.h"
#include "../SpectrumBase.h"

namespace ana::dc {

  class DNCBackground : public SpectrumBase {
   public:
    explicit DNCBackground(std::shared_ptr<io::Options> options);

    ~DNCBackground() override = default;

    bool check_and_recalculate(const ParameterWrapper& parameter) override;

    [[nodiscard]] std::span<const double> get_spectrum(params::dc::DetectorType type) const noexcept override;

   private:
    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;

    using array_t = std::array<double, io::dc::Constants::number_of_energy_bins>;
    using uo_map_t = std::unordered_map<params::dc::DetectorType, array_t>;

    uo_map_t m_Cache;
    uo_map_t m_SpectrumTemplate_Gd;
    uo_map_t m_SpectrumTemplate_Hy;
  };

}  // namespace ana::dc
