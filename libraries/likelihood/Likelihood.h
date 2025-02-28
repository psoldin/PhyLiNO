#pragma once

#include <span>
#include "Options.h"
#include "Parameter.h"

namespace ana::dc {

  class Likelihood {
   public:
    explicit Likelihood(std::shared_ptr<io::Options> options)
      : m_Options(std::move(options)) {}

    virtual ~Likelihood() = default;

    [[nodiscard]] const std::shared_ptr<io::Options>& options() const noexcept { return m_Options; }

    [[nodiscard]] virtual double calculate_likelihood(const double* parameter) = 0;

   protected:
    std::shared_ptr<io::Options> m_Options;
  };

}  // namespace ana::dc
