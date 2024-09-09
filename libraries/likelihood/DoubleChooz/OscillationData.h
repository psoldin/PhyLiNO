#pragma once

// includes
#include "Parameter.h"

// STL includes
#include <span>

namespace ana::dc {

  struct OscillationData {
    using span_t = std::span<const double>;
    OscillationData(span_t loe, span_t scl, int target_bin, params::dc::DetectorType type)
      : LoverE(loe)
      , scaling(scl)
      , target_bin(target_bin)
      , type(type) {}

    const span_t LoverE; /**< The L over E data. */
    const span_t scaling; /**< The scaling data. */
    const int target_bin; /**< The target bin. */
    const params::dc::DetectorType type; /**< The detector type. */
  };

} // namespace ana::dc