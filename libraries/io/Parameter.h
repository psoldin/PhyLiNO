#pragma once

// STL includes
#include <sstream>
#include <string>

namespace params {

  namespace dc {

    /**
     * Enumeration of detector types used in the Double Chooz experiment.
     * Each detector type is represented as a bit flag, which can be combined
     * using bitwise OR to represent multiple detector types.
     */
    enum DetectorType : char {
      FD     = 1 << 0,
      ND     = 1 << 1,
      V1     = 1 << 2,
      V2     = 1 << 3,
      B1     = 1 << 4,
      B2     = 1 << 5,
      FDI    = FD | V1,
      FDII   = FD | V2,
      FDIB1  = FD | V1 | B1,
      FDIB2  = FD | V1 | B2,
      FDIIB1 = FD | V2 | B1,
      FDIIB2 = FD | V2 | B2,
      NDB1   = ND | B1,
      NDB2   = ND | B2
    };

    /**
     * Check if the type is one of the base types, i.e. either ND, FDI or FDII.
     * @param type DetectorType
     * @return boolean whether the types is either ND, FDI, FDII
     */
    constexpr bool is_base_type(DetectorType type) noexcept {
      using enum DetectorType;
      return static_cast<bool>((type == ND) + (type == FDII) + (type == FDI));
    }

    constexpr DetectorType cast_to_base_type(DetectorType type) noexcept {
      using enum DetectorType;
      constexpr int base_type = ND | FDI | FDII;
      return static_cast<DetectorType>(type & base_type);
    }

    /**
     * @brief Casts a DetectorType to a version without reactor split.
     *
     * This function takes a DetectorType and returns a new DetectorType with the reactor split bit removed.
     *
     * @param type The original DetectorType to cast.
     * @return The new DetectorType without the reactor split bit.
     */
    constexpr DetectorType cast_to_no_reactor_split(DetectorType type) noexcept {
      using enum DetectorType;
      constexpr unsigned int base_type = FD | ND | V1 | V2;
      return static_cast<DetectorType>(type & base_type);
    }

    /**
     * Determines if a given detector type is a reactor split detector.
     *
     * @param type The detector type to check.
     * @return True if the detector type is a reactor split detector, false otherwise.
     */
     constexpr bool is_reactor_split(DetectorType type) noexcept {
      using enum DetectorType;
      constexpr int split_type = B1 | B2;
      return static_cast<bool>(type & split_type);
    }

    /**
     * Check if the type is a DataType that is B1 split, i.e. NDB1 for instance.
     * @param type DetectorType
     * @return boolean: Is B1 split
     */
     constexpr bool is_B1_split(DetectorType type) noexcept {
      using enum DetectorType;
      return static_cast<bool>(type & B1);
    }

    /**
     * Returns true if the given detector type is a B2 split detector.
     *
     * @param type The detector type to check.
     * @return True if the detector type is a B2 split detector, false otherwise.
     */
     constexpr bool is_B2_split(DetectorType type) noexcept {
      using enum DetectorType;
      return static_cast<bool>(type & B2);
    }

    /**
     * @brief Casts the given DetectorType to a B1 split type if it is a base type.
     *
     * This function checks if the provided DetectorType is a base type. If it is,
     * it casts the type to a B1 split type by performing a bitwise OR operation
     * with the B1 value. If the type is not a base type, it returns the type as is.
     *
     * @param type The DetectorType to be cast.
     * @return The casted DetectorType if it is a base type, otherwise the original type.
     */
     constexpr DetectorType cast_to_B1_split(DetectorType type) noexcept {
      using enum DetectorType;
      return is_base_type(type) ? static_cast<DetectorType>(type | B1) : type;
    }

    /**
     * @brief Casts the given DetectorType to a B2 split type if it is a base type.
     *
     * This function checks if the provided DetectorType is a base type. If it is,
     * it casts the type to a B2 split type by performing a bitwise OR operation
     * with the B2 value. If the type is not a base type, it returns the type as is.
     *
     * @param type The DetectorType to be cast.
     * @return The casted DetectorType if it is a base type, otherwise the original type.
     */
     constexpr DetectorType cast_to_B2_split(DetectorType type) noexcept {
      using enum DetectorType;
      return is_base_type(type) ? static_cast<DetectorType>(type | B2) : type;
    }

    /**
     * Test whether the DetectorType is of type far detector.
     * @param type DetectorType
     * @return boolean: Is far detector type
     */
     constexpr bool is_far_detector(DetectorType type) noexcept {
      using enum DetectorType;
      return static_cast<bool>(type & FD);
    }

    /**
     * Test whether the DetectorType is of type near detector.
     * @param type DetectorType
     * @return boolean: Is near detector type
     */
     constexpr bool is_near_detector(DetectorType type) noexcept {
      using enum DetectorType;
      return static_cast<bool>(type & ND);
    }

    /**
     * Returns the name of the detector type as a string.
     *
     * @param type The detector type to get the name of.
     * @return The name of the detector type as a string.
     */
     inline std::string get_detector_name(DetectorType type) noexcept {
      std::stringstream ss;
      using enum DetectorType;
      const bool is_fd = is_far_detector(type);
      ss << (is_fd ? "FD" : "ND");
      if (is_fd) {
        ss << ((type & V1) ? "I" : "II");
      }
      if (is_reactor_split(type)) {
        ss << (is_B1_split(type) ? "B1" : "B2");
      }
      return ss.str();
    }

    /**
     * Enumeration of detector types used in the Double Chooz experiment.
     * Each detector type is represented as an integer value.
     */
    enum Detector : int {
      BkgRAcc = 0,
      BkgRLi,
      BkgRFNSM,
      BkgRDNCHy,
      BkgRDNCGd,
      FNSMShape01,
      FNSMShape02,
      FNSMShape03,
      FNSMShape04,
      FNSMShape05,
      FNSMShape06,
      FNSMShape07,
      FNSMShape08,
      FNSMShape09,
      FNSMShape10,
      FNSMShape11,
      FNSMShape12,
      FNSMShape13,
      FNSMShape14,
      FNSMShape15,
      FNSMShape16,
      FNSMShape17,
      FNSMShape18,
      FNSMShape19,
      FNSMShape20,
      FNSMShape21,
      FNSMShape22,
      FNSMShape23,
      FNSMShape24,
      FNSMShape25,
      FNSMShape26,
      FNSMShape27,
      FNSMShape28,
      FNSMShape29,
      FNSMShape30,
      FNSMShape31,
      FNSMShape32,
      FNSMShape33,
      FNSMShape34,
      FNSMShape35,
      FNSMShape36,
      FNSMShape37,
      FNSMShape38,
      FNSMShape39,
      FNSMShape40,
      FNSMShape41,
      FNSMShape42,
      FNSMShape43,
      FNSMShape44,
      EnergyB,
      EnergyC,
      MCNorm,
      NuOffOffRate,
      NuShape01,
      NuShape02,
      NuShape03,
      NuShape04,
      NuShape05,
      NuShape06,
      NuShape07,
      NuShape08,
      NuShape09,
      NuShape10,
      NuShape11,
      NuShape12,
      NuShape13,
      NuShape14,
      NuShape15,
      NuShape16,
      NuShape17,
      NuShape18,
      NuShape19,
      NuShape20,
      NuShape21,
      NuShape22,
      NuShape23,
      NuShape24,
      NuShape25,
      NuShape26,
      NuShape27,
      NuShape28,
      NuShape29,
      NuShape30,
      NuShape31,
      NuShape32,
      NuShape33,
      NuShape34,
      NuShape35,
      NuShape36,
      NuShape37,
      NuShape38,
      NuShape39,
      NuShape40,
      NuShape41,
      NuShape42,
      NuShape43,
      AccShape01,
      AccShape02,
      AccShape03,
      AccShape04,
      AccShape05,
      AccShape06,
      AccShape07,
      AccShape08,
      AccShape09,
      AccShape10,
      AccShape11,
      AccShape12,
      AccShape13,
      AccShape14,
      AccShape15,
      AccShape16,
      AccShape17,
      AccShape18,
      AccShape19,
      AccShape20,
      AccShape21,
      AccShape22,
      AccShape23,
      AccShape24,
      AccShape25,
      AccShape26,
      AccShape27,
      AccShape28,
      AccShape29,
      AccShape30,
      AccShape31,
      AccShape32,
      AccShape33,
      AccShape34,
      AccShape35,
      AccShape36,
      AccShape37,
      AccShape38,
      _last_of_detector_
    };

    /**
     * @brief Enumeration of background types used in the Double Chooz experiment.
     *
     * This enum class represents the different types of backgrounds that can be encountered in the Double Chooz experiment.
     * The background types include accidental, lithium, fast neutron, and delayed neutron capture.
     */
    enum class BackgroundType : int {
      accidental, /**< Accidental background */
      lithium,    /**< Lithium background */
      fastN,      /**< Fast neutron background */
      dnc         /**< Delayed neutron capture background */
    };

    /**
     * Returns the name of the background type as a string.
     *
     * @param type The background type.
     * @return The name of the background type as a string.
     * @throws std::invalid_argument if the background type is invalid.
     */
    inline std::string get_background_name(BackgroundType type) {
      switch (type) {
        case BackgroundType::accidental:
          return "accidental";
        case BackgroundType::lithium:
          return "lithium";
        case BackgroundType::fastN:
          return "fnsm";
        case BackgroundType::dnc:
          return "dnc";
        default:
          throw std::invalid_argument("Invalid background type");
      }
    }

  }  // namespace dc

  /**
   * Enumeration of general parameters used in the Double Chooz experiment.
   * Each parameter is represented as an integer value.
   */
  enum General : int {
    SinSqT13 = 0,
    DeltaM31,
    SinSqT12,
    DeltaM21,
    SinSqT14,
    DeltaM41,
    Bugey4,
    EnergyA,
    LiShape01,
    LiShape02,
    LiShape03,
    LiShape04,
    LiShape05,
    LiShape06,
    LiShape07,
    LiShape08,
    LiShape09,
    LiShape10,
    LiShape11,
    LiShape12,
    LiShape13,
    LiShape14,
    LiShape15,
    LiShape16,
    LiShape17,
    LiShape18,
    LiShape19,
    LiShape20,
    LiShape21,
    LiShape22,
    LiShape23,
    LiShape24,
    LiShape25,
    LiShape26,
    LiShape27,
    LiShape28,
    LiShape29,
    LiShape30,
    LiShape31,
    LiShape32,
    LiShape33,
    LiShape34,
    LiShape35,
    LiShape36,
    LiShape37,
    LiShape38,
    _last_of_General_
  };

  /** Get the number of parameters in the General namespace */
   constexpr int number_of_general_parameters() noexcept {
    constexpr int num = static_cast<int>(_last_of_General_);
    return num;
  }

  /** Get the number of parameters in the DoubleChooz Detector namespace */
   constexpr int number_of_DoubleChooz_detector_parameters() noexcept {
    constexpr int num = static_cast<int>(dc::Detector::_last_of_detector_);
    return num;
  }

  /** Number of data sets that are used in the Likelihood analysis */
   constexpr int number_of_data_sets() noexcept {
    return 3;  // ND, FDI, FDII
  }

  /** Get the total number of parameters used in the Likelihood fit */
   constexpr int number_of_parameters() noexcept {
    return number_of_general_parameters() + number_of_data_sets() * number_of_DoubleChooz_detector_parameters();
  }

  /**
   * Get the index of the detector data set. The parameters are divided into parameter chunks associated with the
   * detector data set. The general parameters come first and then the same parameters for each detector, so to calculate
   * the absolute index, the data sets need to be addressed with an associate index, which is calculated in this function.
   * @param d The detector type
   * @return index of the detector data set: ND, FDI, FDII, NDB1, NDB2, FDIB1, FDIB2, FDIIB1, FDIIB2
   */
   constexpr int get_index(params::dc::DetectorType d) noexcept {
    using enum dc::DetectorType;
    bool is_split = is_reactor_split(d);                                                                // if the reactor data are split, the index is different
    int  idx      = static_cast<bool>(d & FD) & (1 + static_cast<bool>(d & V2));                        // get base type
    return !is_split * idx + is_split * (number_of_data_sets() + 2 * idx + static_cast<bool>(d & B2));  // total index
  }

  /**
   * Calculate the absolute index of the model parameters, specifically for the individual detector parameters.
   * The general parameters are given by their index, so the detector parameters need to be offset.
   * @param d DetectorType
   * @param p Index of the parameter in the Detector namespace
   * @return Absolute index
   */
   constexpr int index(params::dc::DetectorType d, int p) noexcept {
    return number_of_general_parameters() + get_index(d) * number_of_DoubleChooz_detector_parameters() + p;
  }

  /**
   * Get the index of the general parameters. Since the index for these parameters is given by their position in
   * the enum, the function basically returns its input again. This is only a convenience function to have the same
   * interface for all parameters.
   * @param idx
   * @return Index of the general parameter
   */
   constexpr int index(General idx) noexcept {
    return static_cast<int>(idx);
  }

  /**
   * @brief Throws an exception indicating that General parameters are not associated with a detector type.
   *
   * @param d The detector type (dc::DetectorType).
   * @param idx The general index (General).
   * @return This function always throws an exception and never returns a value.
   * @throws std::invalid_argument Always throws this exception with a message indicating the invalid index.
   */
  inline int index(dc::DetectorType d, General idx) {
    throw std::invalid_argument("Invalid Index, General parameters are not associated with a detector type.");
    return -1;
  }

  /**
   * Get the index of the detector parameters. Since the index for these parameters are offset after the general
   * parameters, the index is calculated on the fly. This itself is a convenience function and calls the index function
   * from above.
   * @param d DetectorType
   * @param p Index of the parameter in the Detector index
   * @return Absolute index
   */
   constexpr int index(dc::DetectorType d, dc::Detector p) noexcept {
    return index(d, static_cast<int>(p));
  }
}  // namespace params
