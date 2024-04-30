#pragma once

#include <Parameter.h>

namespace ana {

  // TODO: Documentation missing
  class AnalysisParameter {
   public:
    AnalysisParameter(const double* __restrict__ input_parameter) noexcept
      : m_InputParameter{input_parameter, params::dc::number_of_parameters()} {
    }

    ~AnalysisParameter() = default;

    void set_input_parameter(std::span<const double> input_parameter) noexcept;

    [[nodiscard]] std::span<const double> get_input_parameter() const noexcept;

   private:
    std::span<const double> m_InputParameter;
  };

  class Likelihood {
   public:
    Likelihood();

    double likelihood_function(const double* __restrict__ parameter) noexcept {
      return likelihood_function(AnalysisParameter(parameter));
    }

    double likelihood_function(const AnalysisParameter& parameter) const noexcept {
    }

    AnalysisParameter correlate_parameters(const double* __restrict__ parameter) noexcept;

    ~Likelihood() = default;
  };
}  // namespace ana
