#pragma once

#include "../Definitions.h"

// Eigen include
#include <Eigen/Core>
#include <Eigen/Eigenvalues>

// STL include
#include <span>

namespace ana::dc {

  namespace {
    template <typename T>
    inline auto pow_2(T&& t) noexcept { return t * t; }

    template <int Nbins>
    inline auto make_spectrum(std::span<const double> shape) noexcept {
      return Eigen::Map<const Eigen::Vector<double, Nbins>>(shape.data(), Nbins);
    }
  }

  template <int NBins>
  void calculate_spectrum(double rate,
                          const Eigen::Vector<double, NBins>& shape,
                          std::span<const double> shape_parameter,
                          const Eigen::Matrix<double, NBins, NBins>& covMatrix,
                          return_t<NBins>& result) {

    auto backgroundSpectrum = make_spectrum<NBins>(shape);

    auto backgroundSpectrumSubset = backgroundSpectrum.template head<NBins>();
    auto backgroundOuterProduct = backgroundSpectrumSubset * backgroundSpectrumSubset.transpose();

    // Element-wise multiplication of the fractional covariance matrix with the outer product
    auto defracCovMatrix = pow_2(rate) * covMatrix.cwiseProduct(backgroundOuterProduct);

    using matrix_t = Eigen::Matrix<double, NBins, NBins>;

    Eigen::SelfAdjointEigenSolver<matrix_t> eigen_solver(0.5 * (defracCovMatrix + defracCovMatrix.transpose()));

    // The correction value for the eigenvalue that is iteratively increased until success
    double eigenvalueCorrection = 4e-14;

    auto compute_corrected_eigenvalues = [&eigen_solver](double correction) noexcept {
      return eigen_solver.eigenvalues().unaryExpr([correction](double v) { return std::max(v, correction); }).asDiagonal();
    };

    const auto& eigenvectors = eigen_solver.eigenvectors();
    matrix_t corrected_matrix;
    Eigen::LLT<matrix_t> llt_solver;

    while (true) {
      corrected_matrix = eigenvectors * compute_corrected_eigenvalues(eigenvalueCorrection) * eigenvectors.transpose();
      llt_solver.compute(corrected_matrix);
      if (llt_solver.info() == Eigen::Success) {
        break;
      }
      eigenvalueCorrection *= 1.1;
    }

    auto param_map = make_spectrum<NBins>(shape_parameter);
    Eigen::Vector<double, NBins> shifts = llt_solver.matrixL() * param_map;

    std::transform(backgroundSpectrum.cbegin(), backgroundSpectrum.cend(), result.begin(),
                   [rate](double x) { return std::max(x * rate, 0.0); });

    std::transform(shifts.cbegin(), shifts.cend(), result.cbegin(), result.begin(), std::plus<>());
  }
}