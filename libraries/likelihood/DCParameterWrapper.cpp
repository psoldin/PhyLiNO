#include "DCParameterWrapper.h"

namespace ana::dc {

  DCParameterWrapper::DCParameterWrapper(const double* parameter)
    : ParameterWrapper(parameter) {
    reset_parameter(parameter);
  }

  void DCParameterWrapper::reset_parameter(const double* parameter) {
    m_RawParameter = parameter;
    std::copy(parameter, parameter + params::number_of_parameters(), m_UnifiedParameters.begin());
    unify_parameters();
  }

  void DCParameterWrapper::unify_parameters() {
  }

}  // namespace ana::dc