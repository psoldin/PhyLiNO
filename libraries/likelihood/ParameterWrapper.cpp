#include "ParameterWrapper.h"

namespace ana {

  ParameterWrapper::ParameterWrapper(const double* parameter)
    : m_RawParameter(parameter) {
    std::copy(m_RawParameter, m_RawParameter + params::number_of_parameters(), m_UnifiedParameters.begin());
    unify_parameters();
  }

  void ParameterWrapper::unify_parameters() {
  }

}  // namespace ana