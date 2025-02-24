#pragma once

// includes
#include "Definitions.h"
#include "ParameterWrapper.h"
#include "../io/Options.h"

// STL includes
#include <span>

/**
 * @brief The BackgroundBase class is a base class for background models in the ana namespace.
 *
 * It provides a common interface for derived classes to implement background models.
 *
 * @tparam Derived The derived class type.
 */
namespace ana::dc { // TODO move class to ana namespace

  class SpectrumBase {
   public:
    /**
     * @class SpectrumBase
     * @brief Base class for spectrum calculations.
     *
     * This class provides a base implementation for spectrum calculations in any physics experiment.
     * It is intended to be inherited by specific spectrum classes that implement the actual calculations.
     * The class takes an options object as a parameter in its constructor.
     */
    explicit SpectrumBase(std::shared_ptr<io::Options> options)
      : m_Options(std::move(options)) {
    }

    /**
     * @brief Destructor for the BackgroundBase class.
     */
    virtual ~SpectrumBase() = default;

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
     * This is only necessary if the previous calculation step is dependent on the current one.
     * @return bool A boolean indicating if the spectrum was recalculated.
     */
    [[nodiscard]] virtual bool check_and_recalculate(const ParameterWrapper& parameter) = 0;

    /**
     * @brief Get the spectrum for a specific detector type.
     *
     * This pure virtual function must be implemented by derived classes to return the spectrum data
     * for the given detector type.
     *
     * @param type The detector type for which the spectrum is requested.
     * @return std::span<const double> A span containing the spectrum data.
     * It is a span to incorporate all possible shapes.
     */
    [[nodiscard]] virtual std::span<const double> get_spectrum(params::dc::DetectorType type) const = 0;

   protected:
    std::shared_ptr<io::Options> m_Options;
  };

}  // namespace ana::dc
