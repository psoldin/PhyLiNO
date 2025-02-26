#pragma once

#include "Oscillator.h"

namespace ana::dc {

  class ShapeCorrection : public SpectrumBase {
  public:
    explicit ShapeCorrection(std::shared_ptr<io::Options> options, std::shared_ptr<Oscillator> oscillator);
   ;

    ~ShapeCorrection() override = default;

    bool check_and_recalculate(const ParameterWrapper& parameter) noexcept override;

    [[nodiscard]] std::span<const double> get_spectrum(params::dc::DetectorType type) const noexcept override {
      return m_Cache.at(type);
    }

  private:
    std::shared_ptr<io::Options> m_Options;
    std::shared_ptr<Oscillator> m_Oscillator;

    template <typename T>
    using uo_map = std::unordered_map<params::dc::DetectorType, T>;

    uo_map<Eigen::Array<double, 80, 1>>   m_Cache;
    uo_map<const Eigen::MatrixXd*> m_CovMatrix;

    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;
  };

}