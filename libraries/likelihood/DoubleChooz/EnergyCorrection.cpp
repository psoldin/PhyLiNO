#include "EnergyCorrection.h"
#include "FuzzyCompare.h"
#include <unsupported/Eigen/Splines>
#include <Constants.h>

namespace ana::dc {

  inline double energy_scale_correction(double parA, double parB, double parC, double E) noexcept {
    double result = E;
    if (std::abs(parC) < 1.0e-7) {
      double equationSubterm = (-1.0 * parB) / (2.0 * parC);
      double term            = std::sqrt(std::pow(equationSubterm, 2) - (parA - result) / parC);
      double upperBranch     = equationSubterm + term;
      double lowerBranch     = equationSubterm - term;

      result = (std::abs(upperBranch) < std::abs(lowerBranch)) ? upperBranch : lowerBranch;
    } else if (utilities::fuzzyIsNull(parC)) {
      result = (result - parA) / parB;
    }

    return std::max(0.0, result);
  }

  template <typename T>
  std::vector<T> range(T start, T stop, T step) {
    if (step == static_cast<T>(0)) {
      throw std::invalid_argument("step for range must be non zero");
    }

    std::vector<T> result;

    T i = start;
    while ((step > 0) ? (i < stop) : (i > stop)) {
      result.push_back(i);
      i += step;
    }

    return result;
  }

  class SplineFunction {
   public:
    SplineFunction(Eigen::VectorXd const& x_vec,
                   Eigen::VectorXd const& y_vec)
        : x_min(x_vec.minCoeff())
        , x_max(x_vec.maxCoeff())
        ,
        // Spline fitting here. X values are scaled down to [0, 1] for this.
          spline_(Eigen::SplineFitting<Eigen::Spline<double, 1>>::Interpolate(
              y_vec.transpose(),
              // No more than cubic spline, but accept short vectors.

              3,  //std::min<int>(x_vec.rows() - 1, 3),
              scaled_values(x_vec))) {}

    double operator()(double x) const {
      // x values need to be scaled down in extraction as well.
      const auto tmp = spline_(scaled_value(x))(0);
      return (tmp < 0.0) ? 0.0 : tmp;
      //return spline_(scaled_value(x))(0);
    }

    [[nodiscard]] double derivative(double x) const noexcept {
      return spline_.derivatives(scaled_value(x - x_min), 1)(1);
    }

   private:
    // Helpers to scale X values down to [0, 1]
    [[nodiscard]] inline double scaled_value(double x) const noexcept {
      return (x - x_min) / (x_max - x_min);
    }

    [[nodiscard]] Eigen::RowVectorXd scaled_values(Eigen::VectorXd const& x_vec) const {
      return x_vec.unaryExpr([this](double x) { return scaled_value(x); }).transpose();
    }

    double x_min;
    double x_max;

    // Spline of one-dimensional "points."
    Eigen::Spline<double, 1, 3> spline_;
  };

  EnergyCorrection::EnergyCorrection(std::shared_ptr<io::Options> options)
    : m_Options(std::move(options)) {
    auto xpos_values = range(0.25, 20.25, 0.25);
    m_XPos           = Eigen::Array<double, 80, 1>(xpos_values.data());
  }

  void EnergyCorrection::calculate_energy_correction(const ParameterWrapper& parameter,
                                                     const Oscillator&       oscillator) noexcept {

    using namespace params;
    using namespace params::dc;
    using enum DetectorType;

    const double parA = parameter[General::EnergyA];

    for (auto detector : {ND, FDI, FDII}) {
      const Eigen::Array<double, 80, 1>& oscillatedSpectrum = oscillator.get_spectrum(detector);

      Eigen::Array<double, 80, 1> cumsum;
      std::partial_sum(oscillatedSpectrum.data(), oscillatedSpectrum.data() + oscillatedSpectrum.size(), cumsum.data());

      SplineFunction spline(m_XPos, cumsum);

      const double parB = parameter[index(detector, Detector::EnergyB)];
      const double parC = parameter[index(detector, Detector::EnergyC)];

      auto& energy_corrected_spectrum = m_Cache[detector];

      for (int i = 1; i < io::Constants::number_of_energy_bins; ++i) {
        double e_upper = energy_scale_correction(parA, parB, parC, io::Constants::EnergyBinXaxis[i - 1]);
        double e_lower = energy_scale_correction(parA, parB, parC, io::Constants::EnergyBinXaxis[i]);
        double bin_content = spline(e_upper) - spline(e_lower);
        energy_corrected_spectrum[i - 1] = std::max(0.0, bin_content);
      }
    }
  }

}  // namespace ana::dc
