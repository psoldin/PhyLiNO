#pragma once

#include "OscillationData.h"

namespace ana::dc {

  class RangeOscillator {
   public:
    RangeOscillator() = default;

    virtual ~RangeOscillator() = default;

    [[nodiscard]] double operator()(const OscillationData& data) const noexcept {
      return oscillate_events(data);
    }

   protected:
    [[nodiscard]] virtual double oscillate_events(const OscillationData& data) const noexcept = 0;
  };

}  // namespace ana::dc
