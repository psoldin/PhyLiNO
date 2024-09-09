#include "ParameterWrapper.h"
#include "FuzzyCompare.h"

namespace ana::dc {

  ParameterWrapper::ParameterWrapper(const double* parameter)
    : m_RawParameter(parameter) {
    reset_parameter(parameter);
  }

  void ParameterWrapper::reset_parameter(const double* parameter) {
    m_RawParameter = parameter;
    std::copy(m_UnifiedParameters.cbegin(), m_UnifiedParameters.cend(), m_PreviousParameters.begin());
    std::copy(parameter, parameter + params::number_of_parameters(), m_UnifiedParameters.begin());
    unify_parameters();
  }

  bool ParameterWrapper::parameter_changed(int idx) const noexcept {
    return not utilities::fuzzyCompare(m_UnifiedParameters[idx], m_PreviousParameters[idx]);
  }

  bool ParameterWrapper::range_changed(int start, int end) const noexcept {
    return not std::equal(m_UnifiedParameters.cbegin() + start, m_UnifiedParameters.cbegin() + end,
               m_PreviousParameters.cbegin() + end, m_PreviousParameters.cbegin() + end,
               [](double a, double b) { return utilities::fuzzyCompare(a, b); });
  }

  void ParameterWrapper::unify_parameters() {
  }

}  // namespace ana::dc