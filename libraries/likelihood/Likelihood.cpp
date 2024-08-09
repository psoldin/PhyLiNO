#include "Likelihood.h"
#include "DCParameterWrapper.h"

#include "FuzzyCompare.h"

namespace ana {

  __attribute__((always_inline)) inline bool check_range(int from, int to, const DCParameterWrapper& parameter) noexcept {
    return std::equal(parameter.cbegin() + from, parameter.cbegin() + (to + 1), parameter.raw_parameters().begin() + from, [](double a, double b) { return utilities::fuzzyCompare(a, b); });
  }

  __attribute__((always_inline)) inline bool check_oscillation_parameters(const DCParameterWrapper& parameter) noexcept {
    const bool check_t13 = utilities::fuzzyCompare(parameter[params::General::SinSqT13], parameter.raw_parameters()[params::General::SinSqT13]);
    const bool check_dm31 = utilities::fuzzyCompare(parameter[params::General::DeltaM31], parameter.raw_parameters()[params::General::DeltaM31]);
    return check_t13 & check_dm31;
  }

  Likelihood::Likelihood() {
  }
}  // namespace ana