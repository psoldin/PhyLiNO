#include "ParameterWrapper.h"
#include "FuzzyCompare.h"

namespace ana::dc {

  ParameterWrapper::ParameterWrapper(const double* parameter)
    : m_RawParameter(parameter) {
    reset_parameter(parameter);
  }

  void ParameterWrapper::reset_parameter(const double* parameter) {
    m_RawParameter = parameter;
    std::copy(parameter, parameter + params::number_of_parameters(), m_UnifiedParameters.begin());
    unify_parameters();
  }

  bool ParameterWrapper::parameter_changed(int idx) const noexcept {
    return utilities::fuzzyCompare(m_UnifiedParameters[idx], m_PreviousParameters[idx]);
  }

  void ParameterWrapper::unify_parameters() {
  }

}  // namespace ana::dc