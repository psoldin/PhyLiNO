#pragma once

#include "InputOptions.h"
#include "Parameter.h"
#include "ReactorData.h"

#include <span>
#include <string>
#include <vector>

namespace io {

  /**
   *
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
      [[nodiscard]] std::span<const double> measurement_data(params::dc::DetectorType type) const noexcept {
        if (params::get_index(type) >= m_MeasurementData.size()) {
          return {};
        }
        return m_MeasurementData[params::get_index(type)];
      }

     private:
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
      KeyPair(params::dc::DetectorType k1, params::BackgroundType k2)
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

    std::vector<ReactorData> m_ReactorData;

    std::vector<std::vector<double>> m_SignalData;
    std::vector<std::vector<double>> m_MeasurementData;
  };

}  // namespace io