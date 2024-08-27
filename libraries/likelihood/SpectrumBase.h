#pragma once

#include "Options.h"
#include "DCParameterWrapper.h"

#include <Eigen/Core>

/**
 * @brief The BackgroundBase class is a base class for background models in the ana namespace.
 *
 * It provides a common interface for derived classes to implement background models.
 *
 * @tparam Derived The derived class type.
 */
namespace ana {

  template <typename T>
  concept is_background_derived = requires(T t) {
    { t.check_and_recalculate_spectrum(std::declval<ParameterWrapper>()) } -> std::same_as<void>;
    { t.return_spectrum(params::dc::DetectorType::ND) } -> std::same_as<Eigen::Array<double, 44, 1>>;
  };

  template <typename Derived>
  class BackgroundBase {
   public:
    /**
     * @class BackgroundBase
     * @brief Base class for background calculations.
     *
     * This class provides a base implementation for background calculations in the DoubleChooz experiment.
     * It is intended to be inherited by specific background classes that implement the actual calculations.
     * The class takes an options object as a parameter in its constructor.
     */
    explicit BackgroundBase(std::shared_ptr<io::Options> options)
    : m_Options(std::move(options)) {

    }

    /**
     * @brief Destructor for the BackgroundBase class.
     */
    ~BackgroundBase() = default;

    /**
     * @brief Get the options object.
     *
     * @return const std::shared_ptr<io::Options>& The options object.
     */
    [[nodiscard]] const std::shared_ptr<io::Options>& options() const noexcept { return m_Options; }

    /**
     * @brief Check and recalculate the spectrum.
     *
     * This function is called to check if the spectrum needs to be recalculated and then recalculates it if necessary.
     *
     * @param parameter The parameter object.
     */
    void recalculate_spectrum(const ParameterWrapper& parameter) {
      static_assert(is_background_derived<Derived>, "Derived class must implement check_and_recalculate_spectrum");
      static_cast<Derived*>(this)->check_and_recalculate_spectrum(parameter);
    }

    [[nodiscard]] Eigen::Array<double, 44, 1> get_spectrum(params::dc::DetectorType type) const noexcept {
      static_assert(is_background_derived<Derived>, "Derived class must implement return_spectrum");
      return static_cast<Derived*>(this)->return_spectrum(type);
    }

   protected:
    std::shared_ptr<io::Options> m_Options;
  };

}