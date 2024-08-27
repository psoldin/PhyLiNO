#pragma once

#include <Parameter.h>
#include <span>
#include "Options.h"

namespace ana::dc {

  class Likelihood {
  public:
    explicit Likelihood(std::shared_ptr<io::Options> options)
      : m_Options(std::move(options)) { }

    virtual ~Likelihood() = default;

    [[nodiscard]] const std::shared_ptr<io::Options>& options() const noexcept { return m_Options; }

    [[nodiscard]] double calculate_likelihood(const double* parameter) {
    }

  private:
    std::shared_ptr<io::Options> m_Options;
  };

}  // namespace ana::dc
