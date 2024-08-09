#include "ParameterWrapper.h"

namespace ana {

  ParameterWrapper::ParameterWrapper(const double* parameter) {
      reset_parameter(parameter);
  }

  void ParameterWrapper::reset_parameter(const double* parameter) {
    m_RawParameter = parameter;
    unify_parameters(parameter);
  }

  void ParameterWrapper::unify_parameters(const double* parameter) {
    
  }

}  // namespace ana