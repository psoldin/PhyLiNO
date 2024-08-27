#pragma once

#include "Options.h"

namespace ana {
  // TODO: Implementation & Documentation missing

  class Fit {
   public:
    explicit Fit(std::shared_ptr<io::Options> options);

    ~Fit() = default;

   private:
    std::shared_ptr<io::Options> m_Options;
  };

}  // namespace ana
