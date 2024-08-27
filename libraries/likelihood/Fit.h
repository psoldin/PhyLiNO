#pragma once

#include "Options.h"

namespace ana {
  // TODO: Implementation & Documentation missing

  class Fitter {
   public:
    explicit Fitter(std::shared_ptr<io::Options> options);

    ~Fitter() = default;

   private:
    std::shared_ptr<io::Options> m_Options;
  };

}  // namespace ana
