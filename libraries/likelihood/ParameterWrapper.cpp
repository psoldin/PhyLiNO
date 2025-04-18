#include "ParameterWrapper.h"

#include <numeric>

#include "../utilities/FuzzyCompare.h"

namespace ana::dc {

  ParameterWrapper::ParameterWrapper(const std::size_t nParameter, std::shared_ptr<io::Options> options, transform_fn_t transform_fn)
    : m_CurrentParameters(nParameter, 0.0)
    , m_PreviousParameters(nParameter, 0.0)
    , m_ParameterChanged(nParameter, true)
    , m_NParameter(nParameter)
    , m_Options(std::move(options))
    , m_RawParameter(nullptr)
    , m_TransformFn(transform_fn) {}

  void ParameterWrapper::reset_parameter(const double* parameter) {
    // Set the raw parameter pointer to the new parameter array
    m_RawParameter = parameter;

    // Swap the current parameters with the previous parameters
    std::swap(m_CurrentParameters, m_PreviousParameters);

    // Copy the new parameter values into the current parameters array
    std::copy_n(parameter, m_NParameter, m_CurrentParameters.begin());

    if (m_TransformFn) {
      m_TransformFn(*m_Options, m_CurrentParameters);
    }

    // Update the parameter changed status for each parameter
    for (std::size_t i = 0; i < m_NParameter; ++i) {
      m_ParameterChanged[i] = static_cast<char>(utilities::fuzzyCompare(m_CurrentParameters[i], m_PreviousParameters[i]));
    }
  }

  bool ParameterWrapper::check_parameter_changed(const int idx) const {
    if (idx < 0 || idx >= m_NParameter) {
      throw std::out_of_range("Parameter index out of range");
    }
    const bool same = static_cast<bool>(m_ParameterChanged[idx]);
    return !same;
  }

  bool ParameterWrapper::check_parameter_changed(const int from, const int to) const {
    if (from < 0 || to >= m_NParameter) {
      throw std::out_of_range("Parameter index out of range");
    }

    if (to < from) {
      throw std::invalid_argument("Invalid range");
    }

    const auto same_count = std::accumulate(m_ParameterChanged.begin() + from, m_ParameterChanged.begin() + (to + 1), 0);
    const bool same       = same_count == (to - from + 1);
    return !same;
  }

}  // namespace ana::dc