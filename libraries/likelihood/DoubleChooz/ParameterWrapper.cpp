#include "ParameterWrapper.h"
#include "FuzzyCompare.h"

namespace ana::dc {

  ParameterWrapper::ParameterWrapper(const double* parameter)
    : m_RawParameter(parameter) {
    reset_parameter(parameter);
  }

  void ParameterWrapper::reset_parameter(const double* parameter) {
    m_RawParameter = parameter;
    std::copy(m_CurrentParameters.cbegin(), m_CurrentParameters.cend(), m_PreviousParameters.begin());
    std::copy(parameter, parameter + params::number_of_parameters(), m_CurrentParameters.begin());
    unify_parameters();
  }

  bool ParameterWrapper::check_parameter_changed(int idx) const noexcept {
    bool same = utilities::fuzzyCompare(m_CurrentParameters[idx], m_PreviousParameters[idx]);
    return !same;
  }

  bool ParameterWrapper::check_parameter_changed(int from, int to) const noexcept {
    bool same = std::equal(m_CurrentParameters.cbegin() + from,
                           m_CurrentParameters.cbegin() + (to + 1),
                           m_PreviousParameters.cbegin() + from,
                           [](auto a, auto b) { return utilities::fuzzyCompare(a, b); });

    return !same;
  }

  void ParameterWrapper::unify_parameters() {
  }

}  // namespace ana::dc