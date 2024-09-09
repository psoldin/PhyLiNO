#pragma once

#include "../Definitions.h"
#include "DoubleChooz/ParameterWrapper.h"
#include "Options.h"

#include <Eigen/Core>

/**
 * @brief The BackgroundBase class is a base class for background models in the ana namespace.
 *
 * It provides a common interface for derived classes to implement background models.
 *
 * @tparam Derived The derived class type.
 */
namespace ana::dc {

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
     */
    [[nodiscard]] virtual bool check_and_recalculate_spectra(const ParameterWrapper& parameter) = 0;

    [[nodiscard]] virtual const Eigen::Array<double, 44, 1>& get_spectrum(params::dc::DetectorType type) const noexcept = 0;

   protected:
    std::shared_ptr<io::Options> m_Options;
  };

}  // namespace ana::dc
