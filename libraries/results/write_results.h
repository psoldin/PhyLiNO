#pragma once

#include "Fit.h"

// STL includes
#include <string_view>

namespace result {

  inline void write_results(const ana::Fit& fit, std::string_view name);

}  // namespace result