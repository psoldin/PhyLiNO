#pragma once

#include "Options.h"
#include "ParameterWrapper.h"

namespace ana::dc {

  // TODO Documentation
  class Likelihood {
   public:
    explicit Likelihood(std::shared_ptr<io::Options> options, int nParameter)
      : m_Options(std::move(options))
      , m_Parameter(nParameter, m_Options) {}

    virtual ~Likelihood() = default;

    [[nodiscard]] const std::shared_ptr<io::Options>& options() const noexcept { return m_Options; }

    [[nodiscard]] virtual double calculate_likelihood(const double* parameter) = 0;

    [[nodiscard]] ParameterWrapper& parameter() noexcept { return m_Parameter; }

   protected:
    std::shared_ptr<io::Options> m_Options;  ///< The options object used for likelihood calculation.

    ParameterWrapper m_Parameter;  ///< The parameter wrapper object used for likelihood calculation.
  };

}  // namespace ana::dc
