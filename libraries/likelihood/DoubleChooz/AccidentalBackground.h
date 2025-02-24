#pragma once

#include "../SpectrumBase.h"

namespace ana::dc {

  /**
   * @class AccidentalBackground
   * @brief Represents the accidental background spectrum in the Double Chooz experiment.
   *
   * This class inherits from SpectrumBase and provides functionality to handle
   * the accidental background spectrum. It includes methods to check and recalculate
   * the spectra based on given parameters.
   */
  class AccidentalBackground : public SpectrumBase {
  public:
    explicit AccidentalBackground(std::shared_ptr<io::Options> options);

   /**
    * @brief Default destructor for the AccidentalBackground class.
    *
    * This destructor overrides the base class destructor and is marked as default,
    * indicating that the compiler should generate the default implementation.
    */
    ~AccidentalBackground() override = default;

    /**
     * @brief Checks the current spectra and recalculates them if necessary.
     *
     * This function evaluates the given parameter and determines if the spectra
     * need to be recalculated. If a recalculation is required, it performs the
     * necessary computations to update the spectra.
     *
     * @param parameter The parameter to be checked, encapsulated in a ParameterWrapper object.
     * @return True if the spectra were recalculated, false otherwise.
     */
    bool check_and_recalculate(const ParameterWrapper& parameter) override;

    /**
     * @brief Retrieves the spectrum for a given detector type.
     *
     * This function returns a constant span of doubles representing the spectrum
     * for the specified detector type.
     *
     * @param detector The type of detector for which the spectrum is requested.
     * @return A constant span of doubles representing the spectrum for the specified detector.
     */
    [[nodiscard]] std::span<const double> get_spectrum(params::dc::DetectorType detector) const noexcept override {
      // TODO
      return {};
    }

    [[nodiscard]] std::span<const double> get_background_template(params::dc::DetectorType detector) const noexcept {
      return m_BackgroundTemplate.at(detector);
    }

  private:
    std::map<params::dc::DetectorType, std::array<double, 44>> m_BackgroundTemplate;

    void fill_data();

    void recalculate_spectra(const ParameterWrapper& parameter) {
      // Recalculate the accidental background spectrum
    }

  };

}