#pragma once

// STL includes
#include <sstream>
#include <string>

#include <iostream>

namespace params {

  enum class ExperimentType : char {
    DoubleChooz,
    JUNO
  };

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
    enum class SpectrumType : int {
      reactor,    /**< Reactor spectrum */
      accidental, /**< Accidental background */
      lithium,    /**< Lithium background */
      fastN,      /**< Fast Neutron background */
      dnc         /**< Double Neutron Capture background */
    };

    /**
     * Returns the name of the background type as a string.
     *
     * @param type The background type.
     * @return The name of the background type as a string.
     * @throws std::invalid_argument if the background type is invalid.
     */
    inline std::string get_background_name(SpectrumType type) {
      switch (type) {
        case SpectrumType::accidental:
          return "accidental";
        case SpectrumType::lithium:
          return "lithium";
        case SpectrumType::fastN:
          return "fnsm";
        case SpectrumType::dnc:
          return "dnc";
        default:
          throw std::invalid_argument("Invalid background type");
      }
    }

  }  // namespace dc

  /**
   * Enumeration of general parameters used in the Double Chooz experiment.
   * This can also be extended to general parameters used in other experiments.
   * Each parameter is represented as an integer value.
   */
  enum General : int {
    SinSqT13 = 0,
    DeltaMee,
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

  inline std::string get_general_parameter_name(unsigned int c) {
    switch (c) {
      case SinSqT13:
        return "SinSqT13";
      case DeltaMee:
        return "DeltaMee";
      case SinSqT12:
        return "SinSqT12";
      case DeltaM21:
        return "DeltaMSq21";
      case SinSqT14:
        return "SinSqT14";
      case DeltaM41:
        return "DeltaM41";
      case Bugey4:
        return "Bugey4";
      case EnergyA:
        return "EnergyA";
      case LiShape01:
        return "LiShape01";
      case LiShape02:
        return "LiShape02";
      case LiShape03:
        return "LiShape03";
      case LiShape04:
        return "LiShape04";
      case LiShape05:
        return "LiShape05";
      case LiShape06:
        return "LiShape06";
      case LiShape07:
        return "LiShape07";
      case LiShape08:
        return "LiShape08";
      case LiShape09:
        return "LiShape09";
      case LiShape10:
        return "LiShape10";
      case LiShape11:
        return "LiShape11";
      case LiShape12:
        return "LiShape12";
      case LiShape13:
        return "LiShape13";
      case LiShape14:
        return "LiShape14";
      case LiShape15:
        return "LiShape15";
      case LiShape16:
        return "LiShape16";
      case LiShape17:
        return "LiShape17";
      case LiShape18:
        return "LiShape18";
      case LiShape19:
        return "LiShape19";
      case LiShape20:
        return "LiShape20";
      case LiShape21:
        return "LiShape21";
      case LiShape22:
        return "LiShape22";
      case LiShape23:
        return "LiShape23";
      case LiShape24:
        return "LiShape24";
      case LiShape25:
        return "LiShape25";
      case LiShape26:
        return "LiShape26";
      case LiShape27:
        return "LiShape27";
      case LiShape28:
        return "LiShape28";
      case LiShape29:
        return "LiShape29";
      case LiShape30:
        return "LiShape30";
      case LiShape31:
        return "LiShape31";
      case LiShape32:
        return "LiShape32";
      case LiShape33:
        return "LiShape33";
      case LiShape34:
        return "LiShape34";
      case LiShape35:
        return "LiShape35";
      case LiShape36:
        return "LiShape36";
      case LiShape37:
        return "LiShape37";
      case LiShape38:
        return "LiShape38";
      default:
        throw std::invalid_argument(std::to_string(static_cast<int>(c)) + " Not a parameter");
    }
  }

  inline std::string get_detector_parameter_name(unsigned int c) {
    using namespace dc;
    switch (c) {
      case BkgRAcc:
        return "BkgRAcc";
      case BkgRLi:
        return "BkgRLi";
      case BkgRFNSM:
        return "BkgRFNSM";
      case BkgRDNCGd:
        return "BkgRDNCGd";
      case BkgRDNCHy:
        return "BkgRDNCHy";
      case FNSMShape01:
        return "FNSMShape01";
      case FNSMShape02:
        return "FNSMShape02";
      case FNSMShape03:
        return "FNSMShape03";
      case FNSMShape04:
        return "FNSMShape04";
      case FNSMShape05:
        return "FNSMShape05";
      case FNSMShape06:
        return "FNSMShape06";
      case FNSMShape07:
        return "FNSMShape07";
      case FNSMShape08:
        return "FNSMShape08";
      case FNSMShape09:
        return "FNSMShape09";
      case FNSMShape10:
        return "FNSMShape10";
      case FNSMShape11:
        return "FNSMShape11";
      case FNSMShape12:
        return "FNSMShape12";
      case FNSMShape13:
        return "FNSMShape13";
      case FNSMShape14:
        return "FNSMShape14";
      case FNSMShape15:
        return "FNSMShape15";
      case FNSMShape16:
        return "FNSMShape16";
      case FNSMShape17:
        return "FNSMShape17";
      case FNSMShape18:
        return "FNSMShape18";
      case FNSMShape19:
        return "FNSMShape19";
      case FNSMShape20:
        return "FNSMShape20";
      case FNSMShape21:
        return "FNSMShape21";
      case FNSMShape22:
        return "FNSMShape22";
      case FNSMShape23:
        return "FNSMShape23";
      case FNSMShape24:
        return "FNSMShape24";
      case FNSMShape25:
        return "FNSMShape25";
      case FNSMShape26:
        return "FNSMShape26";
      case FNSMShape27:
        return "FNSMShape27";
      case FNSMShape28:
        return "FNSMShape28";
      case FNSMShape29:
        return "FNSMShape29";
      case FNSMShape30:
        return "FNSMShape30";
      case FNSMShape31:
        return "FNSMShape31";
      case FNSMShape32:
        return "FNSMShape32";
      case FNSMShape33:
        return "FNSMShape33";
      case FNSMShape34:
        return "FNSMShape34";
      case FNSMShape35:
        return "FNSMShape35";
      case FNSMShape36:
        return "FNSMShape36";
      case FNSMShape37:
        return "FNSMShape37";
      case FNSMShape38:
        return "FNSMShape38";
      case FNSMShape39:
        return "FNSMShape39";
      case FNSMShape40:
        return "FNSMShape40";
      case FNSMShape41:
        return "FNSMShape41";
      case FNSMShape42:
        return "FNSMShape42";
      case FNSMShape43:
        return "FNSMShape43";
      case FNSMShape44:
        return "FNSMShape44";
      case EnergyB:
        return "EnergyB";
      case EnergyC:
        return "EnergyC";
      case MCNorm:
        return "MCNorm";
      case NuShape01:
        return "NuShape01";
      case NuShape02:
        return "NuShape02";
      case NuShape03:
        return "NuShape03";
      case NuShape04:
        return "NuShape04";
      case NuShape05:
        return "NuShape05";
      case NuShape06:
        return "NuShape06";
      case NuShape07:
        return "NuShape07";
      case NuShape08:
        return "NuShape08";
      case NuShape09:
        return "NuShape09";
      case NuShape10:
        return "NuShape10";
      case NuShape11:
        return "NuShape11";
      case NuShape12:
        return "NuShape12";
      case NuShape13:
        return "NuShape13";
      case NuShape14:
        return "NuShape14";
      case NuShape15:
        return "NuShape15";
      case NuShape16:
        return "NuShape16";
      case NuShape17:
        return "NuShape17";
      case NuShape18:
        return "NuShape18";
      case NuShape19:
        return "NuShape19";
      case NuShape20:
        return "NuShape20";
      case NuShape21:
        return "NuShape21";
      case NuShape22:
        return "NuShape22";
      case NuShape23:
        return "NuShape23";
      case NuShape24:
        return "NuShape24";
      case NuShape25:
        return "NuShape25";
      case NuShape26:
        return "NuShape26";
      case NuShape27:
        return "NuShape27";
      case NuShape28:
        return "NuShape28";
      case NuShape29:
        return "NuShape29";
      case NuShape30:
        return "NuShape30";
      case NuShape31:
        return "NuShape31";
      case NuShape32:
        return "NuShape32";
      case NuShape33:
        return "NuShape33";
      case NuShape34:
        return "NuShape34";
      case NuShape35:
        return "NuShape35";
      case NuShape36:
        return "NuShape36";
      case NuShape37:
        return "NuShape37";
      case NuShape38:
        return "NuShape38";
      case NuShape39:
        return "NuShape39";
      case NuShape40:
        return "NuShape40";
      case NuShape41:
        return "NuShape41";
      case NuShape42:
        return "NuShape42";
      case NuShape43:
        return "NuShape43";
      case AccShape01:
        return "AccShape01";
      case AccShape02:
        return "AccShape02";
      case AccShape03:
        return "AccShape03";
      case AccShape04:
        return "AccShape04";
      case AccShape05:
        return "AccShape05";
      case AccShape06:
        return "AccShape06";
      case AccShape07:
        return "AccShape07";
      case AccShape08:
        return "AccShape08";
      case AccShape09:
        return "AccShape09";
      case AccShape10:
        return "AccShape10";
      case AccShape11:
        return "AccShape11";
      case AccShape12:
        return "AccShape12";
      case AccShape13:
        return "AccShape13";
      case AccShape14:
        return "AccShape14";
      case AccShape15:
        return "AccShape15";
      case AccShape16:
        return "AccShape16";
      case AccShape17:
        return "AccShape17";
      case AccShape18:
        return "AccShape18";
      case AccShape19:
        return "AccShape19";
      case AccShape20:
        return "AccShape20";
      case AccShape21:
        return "AccShape21";
      case AccShape22:
        return "AccShape22";
      case AccShape23:
        return "AccShape23";
      case AccShape24:
        return "AccShape24";
      case AccShape25:
        return "AccShape25";
      case AccShape26:
        return "AccShape26";
      case AccShape27:
        return "AccShape27";
      case AccShape28:
        return "AccShape28";
      case AccShape29:
        return "AccShape29";
      case AccShape30:
        return "AccShape30";
      case AccShape31:
        return "AccShape31";
      case AccShape32:
        return "AccShape32";
      case AccShape33:
        return "AccShape33";
      case AccShape34:
        return "AccShape34";
      case AccShape35:
        return "AccShape35";
      case AccShape36:
        return "AccShape36";
      case AccShape37:
        return "AccShape37";
      case AccShape38:
        return "AccShape38";
      default:
        throw std::invalid_argument("Not a parameter");
    }
  }

  inline std::vector<std::string> get_all_parameter_names() {
    std::vector<std::string> parameter_names;

    for (std::size_t i = 0; i < number_of_general_parameters(); ++i) {
      parameter_names.push_back(get_general_parameter_name(i));
    }

    using namespace dc;

    const std::array detector_types = {ND, FDI, FDII};

    for (auto& detector : detector_types) {
      const auto detector_name = get_detector_name(detector);

      for (std::size_t i = 0; i < number_of_DoubleChooz_detector_parameters(); ++i) {
        parameter_names.push_back(detector_name + "_" + get_detector_parameter_name(i));
      }
    }

    return parameter_names;
  }
}  // namespace params
