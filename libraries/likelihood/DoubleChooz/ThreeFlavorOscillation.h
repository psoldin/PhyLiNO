#pragma once

#include "OscillationData.h"
#include "RangeOscillator.h"

namespace ana::dc {

  class ThreeFlavorOscillation : public RangeOscillator {
  public:
    ThreeFlavorOscillation(double t13, double dmee, double t12, double dm21);

    ~ThreeFlavorOscillation() override = default;

  protected:
    [[nodiscard]] double oscillate_events(const OscillationData& data) const noexcept override;

  private:
    double m_t13;
    double m_dmee;
    double m_t12;
    double m_dm21;
    double m_cos413;
  };

}