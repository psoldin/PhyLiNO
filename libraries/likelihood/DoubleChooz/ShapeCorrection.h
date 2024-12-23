#pragma once
#include "EnergyCorrection.h"
#include "Oscillator.h"

namespace ana::dc {

  class ShapeCorrection {
  public:
    explicit ShapeCorrection(std::shared_ptr<io::Options> options, std::shared_ptr<Oscillator> oscillator);

    ~ShapeCorrection() = default;

    void check_and_recalculate_spectra(const ParameterWrapper& parameter) noexcept;

    [[nodiscard]] const auto& get_spectrum(params::dc::DetectorType type) const noexcept {
      return m_Cache.at(type);
    }

  private:
    std::shared_ptr<io::Options> m_Options;
    std::shared_ptr<ana::dc::Oscillator> m_Oscillator;

    template <typename T>
    using uo_map = std::unordered_map<params::dc::DetectorType, T>;

    uo_map<Eigen::Array<double, 80, 1>>   m_Cache;
    uo_map<Eigen::Matrix<double, 43, 43>> m_CovMatrix;

    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;
  };

}