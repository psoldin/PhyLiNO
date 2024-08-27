#pragma once

#include "Options.h"
#include "Likelihood.h"

namespace ana {
  // TODO: Implementation & Documentation missing

  class Fit {
   public:
    explicit Fit(std::shared_ptr<io::Options> options);

    ~Fit() = default;

    [[nodiscard]] std::shared_ptr<dc::Likelihood> doublechooz_likelihood() const noexcept { return m_DCLikelihood; }

   private:
    std::shared_ptr<io::Options> m_Options;

    std::shared_ptr<dc::Likelihood> m_DCLikelihood;
  };

}  // namespace ana
