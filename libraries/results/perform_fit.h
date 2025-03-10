#pragma once

#include "Fit.h"
#include "write_results.h"

// STL includes
#include <functional>
#include <string_view>

namespace result {

  inline void perform_fit(ana::Fit& fit, std::string_view name, const std::function<void(ana::Fit&)>& function = [](ana::Fit&) {}) {
    function(fit);
    fit.minimize();
    write_results(fit, name);
  }

}  // namespace result
