#pragma once

#include "Options.h"

namespace ana {

  class Fitter {
   public:
    explicit Fitter(io::Options& options);

    ~Fitter() = default;

   private:
    const io::Options& m_Options;
  };

}  // namespace ana