#pragma once

#include "Options.h"

namespace ana {
  // TODO: Implementation & Documentation missing

  class Fitter {
   public:
    explicit Fitter(io::Options& options);

    ~Fitter() = default;

   private:
    const io::Options& m_Options;
  };

}  // namespace ana
