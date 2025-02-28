#pragma once

#include "../InputOptions.h"
#include "../Parameter.h"
#include "../ReactorData.h"

#include <span>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <TMatrixD.h>

#include <Eigen/Core>

namespace io::dc {

  enum class SpectrumType {
    Reactor,
    Accidental,
    FastN,
    Lithium,
    DNC
  };

  /**
   * \brief Class representing the database for Double Chooz data.
   *
   * This class provides access to various types of data related to the Double Chooz experiment,
   * including measurement data, reactor data, and covariance matrices.
   */
  class DataBase {
   public:
    /**
     * Constructor
     * @param inputs InputOptions
     */
    explicit DataBase(const io::InputOptions& inputs);

    /** Default destructor */
    ~DataBase() = default;

    /**
     * Returns a read-only span of measurement data for the specified detector type.
     * @param type The detector type.
     * @return A read-only span of measurement data.
     */
    [[nodiscard]] std::span<const double> measurement_data(params::dc::DetectorType type) const {
      if (params::get_index(type) >= m_MeasurementData.size()) {
        return {};
      }
      return m_MeasurementData[params::get_index(type)];
    }

    /**
     * @brief Accessor function to retrieve the reactor data for a specific detector type.
     *
     * @param type The detector type for which to retrieve the reactor data.
     * @return A reference to the reactor data for the specified detector type.
     */
    [[nodiscard]] const ReactorData& reactor_data(params::dc::DetectorType type) const {
      if (!m_ReactorData.contains(type)) {
        throw std::invalid_argument("Detector type not found in reactor data");
      }

      const auto reactor_data = m_ReactorData.at(type);

      if (reactor_data == nullptr) {
        throw std::invalid_argument("Reactor data is null");
      }

      return *reactor_data;
    }

    [[nodiscard]] std::shared_ptr<Eigen::MatrixXd> covariance_matrix(params::dc::DetectorType detectorType, SpectrumType spectrumType) const;

    [[nodiscard]] const TMatrixD& energy_correlation_matrix() const { return m_EnergyCorrelationMatrix; }

    [[nodiscard]] const TMatrixD& mcNorm_correlation_matrix() const { return m_MCNormCorrelationMatrix; }

    [[nodiscard]] const TMatrixD& interDetector_correlation_matrix() const { return m_InterDetectorCorrelationMatrix; }

    [[nodiscard]] double off_lifetime(params::dc::DetectorType type) const noexcept { return m_OffLifeTime.at(type); }

    [[nodiscard]] double on_lifetime(params::dc::DetectorType type) const noexcept { return m_OnLifeTime.at(type); }

    [[nodiscard]] std::span<const double> background_data(params::dc::BackgroundType type) const {
      return m_BackgroundData.at(type);
    }

    [[nodiscard]] std::pair<double, double> energy_central_values(int idx) const {
      if (!m_EnergyCentralValues.contains(idx)) {
        throw std::invalid_argument("Index not found in energy central values");
      }
      return m_EnergyCentralValues.at(idx);
    }

   private:
    void construct_energy_correlation_matrix();

    const io::InputOptions& m_InputOptions;

    /**
     * @brief A class representing a pair of keys used in the database.
     *
     * This class is used to represent a pair of keys in the database. It contains two keys:
     * a detector type and a background type. The keys are used to uniquely identify a specific
     * entry in the database.
     */
    class KeyPair {
     public:
      /**
       * @brief Constructs a new KeyPair object with the given detector and background types.
       *
       * @param k1 The detector type.
       * @param k2 The background type.
       */
      KeyPair(params::dc::DetectorType k1, params::dc::BackgroundType k2)
        : key1(params::get_index(k1))
        , key2(static_cast<int>(k2)) {}

      /**
       * @brief Compares two KeyPair objects.
       *
       * @param other The KeyPair object to compare to.
       * @return true if this KeyPair is less than the other KeyPair, false otherwise.
       */
      bool operator<(const KeyPair& other) const noexcept {
        return std::tie(key1, key2) < std::tie(other.key1, other.key2);
      }

     private:
      int key1; /**< The first key. */
      int key2; /**< The second key. */
    };

    std::unordered_map<params::dc::DetectorType, std::shared_ptr<ReactorData>> m_ReactorData;

    std::unordered_map<params::dc::BackgroundType, std::vector<double>> m_BackgroundData;

    std::unordered_map<params::dc::DetectorType, double> m_OnLifeTime;
    std::unordered_map<params::dc::DetectorType, double> m_OffLifeTime;

    std::unordered_map<int, std::pair<double, double>> m_EnergyCentralValues;

    std::vector<std::vector<double>> m_SignalData;
    std::vector<std::vector<double>> m_MeasurementData;

    struct KeyHash {
      template <typename T1, typename T2>
      std::size_t operator()(const std::tuple<T1, T2>& key) const {
        auto h1 = std::hash<T1>{}(std::get<0>(key));
        auto h2 = std::hash<T2>{}(std::get<1>(key));
        return h1 ^ (h2 << 1);
      }
    };

    using tuple_t      = std::tuple<params::dc::DetectorType, SpectrumType>;
    using cov_matrix_t = std::shared_ptr<Eigen::MatrixXd>;
    std::unordered_map<tuple_t, cov_matrix_t, KeyHash> m_CovarianceMatrices;
    TMatrixD                                           m_EnergyCorrelationMatrix;         // TODO Replace with Eigen Matrix
    TMatrixD                                           m_MCNormCorrelationMatrix;         // TODO Replace with Eigen Matrix
    TMatrixD                                           m_InterDetectorCorrelationMatrix;  // TODO Replace with Eigen Matrix
  };

}  // namespace io::dc
