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
    inline auto pow_2(T&& t) noexcept {
      return t * t;
    }

    inline auto make_spectrum(std::span<const double> shape) noexcept {
      return Eigen::Map<const Eigen::VectorXd>(shape.data(), shape.size());
    }
  }  // namespace

  inline void calculate_spectrum(double                  rate,
                                 std::span<const double> shape,
                                 std::span<const double> shape_parameter,
                                 const Eigen::MatrixXd&  covMatrix,
                                 std::span<double>       result) {
    auto backgroundSpectrum = make_spectrum(shape);

    auto backgroundSpectrumSubset = backgroundSpectrum.head(covMatrix.rows());

    auto backgroundOuterProduct = backgroundSpectrumSubset * backgroundSpectrumSubset.transpose();

    // Element-wise multiplication of the fractional covariance matrix with the outer product
    auto defracCovMatrix = pow_2(rate) * covMatrix.cwiseProduct(backgroundOuterProduct);

    using matrix_t = Eigen::MatrixXd;

    Eigen::SelfAdjointEigenSolver<matrix_t> eigen_solver(0.5 * (defracCovMatrix + defracCovMatrix.transpose()));

    // The correction value for the eigenvalue that is iteratively increased until success
    double eigenvalueCorrection = 4e-14;

    // Function to compute the corrected eigenvalues
    auto compute_corrected_eigenvalues = [&eigen_solver](double correction) noexcept {
      return eigen_solver.eigenvalues().unaryExpr([correction](double v) { return std::max(v, correction); }).asDiagonal();
    };

    // Get the eigenvectors of the rescaled covariance matrix
    const auto& eigenvectors = eigen_solver.eigenvectors();

    matrix_t             corrected_matrix;
    Eigen::LLT<matrix_t> llt_solver;

    // Iteratively increase the correction value until the matrix is positive definite
    // Compute the Cholesky decomposition of the corrected matrix
    while (true) {
      corrected_matrix = eigenvectors * compute_corrected_eigenvalues(eigenvalueCorrection) * eigenvectors.transpose();
      llt_solver.compute(corrected_matrix);
      if (llt_solver.info() == Eigen::Success) {
        break;
      }
      eigenvalueCorrection *= 1.1;
    }

    auto param_map = make_spectrum(shape_parameter);

    Eigen::VectorXd shifts = llt_solver.matrixL() * param_map;

    // Copy the background spectrum to the result, scaled by the rate
    std::ranges::transform(std::as_const(backgroundSpectrum), result.begin(),
                           [rate](double x) { return std::max(x * rate, 0.0); });

    // Add the shifts to the result
    std::transform(shifts.cbegin(), shifts.cend(), result.begin(), result.begin(), std::plus<>());
  }
}  // namespace ana::dc