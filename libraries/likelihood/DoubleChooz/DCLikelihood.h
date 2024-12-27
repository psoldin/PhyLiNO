#pragma once

#include "../Likelihood.h"
#include "DCBackground.h"
#include "ParameterWrapper.h"

namespace ana::dc {

  class DCLikelihood : public Likelihood {
   public:
    explicit DCLikelihood(std::shared_ptr<io::Options> options)
      : Likelihood(std::move(options))
      , m_Parameter(params::number_of_parameters())
    {}

    ~DCLikelihood() override = default;

    [[nodiscard]] double calculate_likelihood(const double* parameter) override;

    [[nodiscard]] std::span<const double> get_measurement_data(params::dc::DetectorType type) const noexcept {
      return m_MeasurementData.at(type);
    }

    [[nodiscard]] double calculate_off_off_likelihood(const return_t& bkg, params::dc::DetectorType type);

   private:
    [[nodiscard]] double calculate_default_likelihood(const ParameterWrapper& parameter) noexcept;

    [[nodiscard]] double calculate_reactor_split_likelihood(const ParameterWrapper& parameter) noexcept;

    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;

    ParameterWrapper m_Parameter;
    DCBackground     m_Background;

    std::vector<SpectrumBase*> m_Spectra;

    std::unordered_map<params::dc::DetectorType, std::array<double, 44>> m_MeasurementData;
  };

}  // namespace ana::dc
