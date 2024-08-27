#pragma once

#include "../Likelihood.h"

namespace ana::dc {

  class DCLikelihood : public Likelihood {
   public:
    explicit DCLikelihood(std::shared_ptr<io::Options> options)
      : Likelihood(std::move(options)) { }

    ~DCLikelihood() override = default;

    [[nodiscard]] double calculate_likelihood(const double* parameter) override {
      return 0.0;
    }
  };

} // namespace ana::dc