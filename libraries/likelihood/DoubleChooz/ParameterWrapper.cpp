#include "ParameterWrapper.h"

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

  void ParameterWrapper::unify_parameters() {
  }

}  // namespace ana::dc