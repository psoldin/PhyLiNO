#include "ParameterWrapper.h"

namespace ana {

  ParameterWrapper::ParameterWrapper(const double* parameter)
    : m_Parameter(parameter) {
    std::copy(m_Parameter, m_Parameter + params::number_of_parameters(), m_UnifiedParameters.begin());
    unify_parameters();
  }

  void ParameterWrapper::unify_parameters() {
  }

}  // namespace ana