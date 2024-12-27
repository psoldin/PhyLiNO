#include "ParameterWrapper.h"

#include <numeric>

#include "../utilities/FuzzyCompare.h"

namespace ana::dc {

  ParameterWrapper::ParameterWrapper(std::size_t nParameter)
    : m_CurrentParameters(nParameter, 0.0)
    , m_PreviousParameters(nParameter, 0.0)
    , m_ParameterChanged(nParameter, false)
    , m_NParameter(nParameter)
    , m_RawParameter(nullptr) { }

  void ParameterWrapper::reset_parameter(const double* parameter) {
    m_RawParameter = parameter;
    std::swap(m_CurrentParameters, m_PreviousParameters);
    std::copy_n(parameter, m_NParameter, m_CurrentParameters.begin());

    for (std::size_t i = 0; i < m_NParameter; ++i) {
      m_ParameterChanged[i] = static_cast<char>(utilities::fuzzyCompare(m_CurrentParameters[i], m_PreviousParameters[i]));
    }
  }

  bool ParameterWrapper::check_parameter_changed(int idx) const noexcept {
    const bool same = static_cast<bool>(m_ParameterChanged[idx]);
    return !same;
  }

  bool ParameterWrapper::check_parameter_changed(int from, int to) const noexcept {
    const auto same_count = std::accumulate(m_ParameterChanged.begin() + from, m_ParameterChanged.begin() + (to + 1), 0);
    const bool same = same_count == (to - from + 1);
    return !same;
  }

  void ParameterWrapper::unify_parameters() {
  }

}  // namespace ana::dc