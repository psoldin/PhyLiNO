#include "EnergyCorrection.h"
#include <DoubleChooz/Constants.h>
#include <unsupported/Eigen/Splines>
#include "FuzzyCompare.h"
#include "ParameterValue.h"

namespace ana::dc {

  inline double energy_scale_correction(double energyA, double energyB, double energyC, double energyInput) noexcept {
    double result = energyInput;

    // ---- step 1: do the shift as the fitter routine wants it
    // (our reference is the data spectrum, hence inverse function has to be applied to MC) ----
    // If a proper quadratic equation is given, then use p-q-equation
    // (if parameter c is too close to zero, we try to avoid numerical imprecisions by switching to linear equation)
    if (energyC < -0.0000001 || 0.0000001 < energyC) {
      const double equationSubterm = (-1.0 * energyB) / (2.0 * energyC);
      const double term            = std::sqrt(std::pow(equationSubterm, 2) - (energyA - result) / energyC);
      const double upperBranch     = equationSubterm + term;
      const double lowerBranch     = equationSubterm - term;

      // for our parameter set,
      // we have either a solution around 10 MeV or around +-10.000MeV,
      // thus we take the solution with smaller absolute value

      result = (std::abs(upperBranch) < std::abs(lowerBranch)) ? upperBranch : lowerBranch;
    } else if (energyB != 0.0) {
      result = (result - energyA) / energyB;
    }

    //---- step 2: counteract a possible energy corrected MC spectrum ----
    // result = dMCenergyparama + dMCenergyparamb * result + dMCEnergyC * std::pow(result, 2);

    //---- step 3: handle unphysical large corrections ----
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

          3,  // std::min<int>(x_vec.rows() - 1, 3),
          scaled_values(x_vec))) {}

    double operator()(double x) const {
      // x values need to be scaled down in extraction as well.
      const auto tmp = spline_(scaled_value(x))(0);
      return (tmp < 0.0) ? 0.0 : tmp;
      // return spline_(scaled_value(x))(0);
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

  EnergyCorrection::EnergyCorrection(std::shared_ptr<io::Options> options, std::shared_ptr<ShapeCorrection> shape_correction)
    : SpectrumBase(std::move(options))
    , m_ShapeCorrection(std::move(shape_correction)) {
    auto xpos_values = range(0.25, 20.25, 0.25);
    m_XPos           = Eigen::Array<double, 80, 1>(xpos_values.data());

    using enum params::dc::DetectorType;
    for (auto detector : {ND, FDI, FDII}) {
      std::array<double, 44> empty{};
      std::ranges::fill(empty, 0.0);
      m_Cache[detector] = empty;
    }
  }

  [[nodiscard]] inline bool parameter_changed(const ParameterWrapper& parameter) noexcept {
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;
    using namespace params;

    bool has_changed = parameter.check_parameter_changed(EnergyA);
    for (const auto detector : {ND, FDI, FDII}) {
      has_changed |= parameter.check_parameter_changed(index(detector, EnergyB));
      has_changed |= parameter.check_parameter_changed(index(detector, EnergyC));
    }
    return has_changed;
  }

  bool EnergyCorrection::check_and_recalculate(const ParameterWrapper& parameter) noexcept {
    const bool previous_step = m_ShapeCorrection->check_and_recalculate(parameter);
    const bool this_step     = parameter_changed(parameter);
    const bool recalculate   = previous_step | this_step;

    if (recalculate) {
      calculate_spectra(parameter);
    }

    return recalculate;
  }

  std::span<const double> EnergyCorrection::get_spectrum(params::dc::DetectorType type) const noexcept {
    return m_Cache.at(type);
  }

  void EnergyCorrection::calculate_spectra(const ParameterWrapper& parameter) noexcept {
    using namespace params;
    using namespace params::dc;
    using enum DetectorType;

    const auto& db = m_Options->double_chooz().dataBase();

    using span_t = std::span<const double>;

    const auto [CVa, SIGa] = db.energy_central_values(EnergyA);
    const double parA      = CVa + SIGa * parameter[EnergyA];

    for (const auto detector : {ND, FDI, FDII}) {
      span_t oscillated_spectrum = m_ShapeCorrection->get_spectrum(detector);

      Eigen::Array<double, 80, 1> cumSum;  // Cumulative sum of the oscillated spectrum

      // Calculate the cumulative sum of the oscillated spectrum.
      // This makes the integral calculation easier later.
      std::partial_sum(oscillated_spectrum.begin(), oscillated_spectrum.end(), cumSum.begin());

      // Create a spline function from the cumulative sum upper bin edges
      SplineFunction spline(m_XPos, cumSum);

      // Get the central values of the energy correction parameters.
      // The fit parameters are only a deviation from these parameters. This is mathematically equivalent to using
      // the central values and the fit parameters directly. The benefit is that it is numerically more stable.
      const auto [CVb, SIGb] = db.energy_central_values(index(detector, EnergyB));
      const auto [CVc, SIGc] = db.energy_central_values(index(detector, EnergyC));

      // Calculate the energy correction parameters for EnergyB and EnergyC
      const double parB = CVb + SIGb * parameter[index(detector, EnergyB)];
      const double parC = CVc + SIGc * parameter[index(detector, EnergyC)];

      // Get the edges for the energy bins
      const auto& binning = io::dc::Constants::EnergyBinXaxis;

      // Get the cache for the detector as reference
      auto& energy_corrected_spectrum = m_Cache[detector];

      // The way the correction is implemented is that the bin edges are used to calculate the energy correction.
      // The bin content is then calculated by integrating the spline function over the new bin edges.
      for (int i = 1; i < io::dc::Constants::number_of_energy_bins; ++i) {
        const double e_lower = energy_scale_correction(parA, parB, parC, binning[i - 1]);
        const double e_upper = energy_scale_correction(parA, parB, parC, binning[i]);

        // Calculate the bin content by "integrating" the spline function over the new bin edges.
        // The bin content is the difference between the spline value at the upper and lower bin edges since this
        // is the cumulative sum of the oscillated spectrum.
        const double bin_content = spline(e_upper) - spline(e_lower);

        // Store the bin content in the cache and limit it to positive values.
        energy_corrected_spectrum[i - 1] = std::max(0.0, bin_content);
      }
    }
  }

}  // namespace ana::dc
