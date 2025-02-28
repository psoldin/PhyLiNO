#pragma once

#include "../Likelihood.h"
#include "Options.h"
#include "ParameterWrapper.h"
#include "TVectorD.h"

#include "AccidentalBackground.h"
#include "DNCBackground.h"
#include "FastNBackground.h"
#include "LithiumBackground.h"
#include "ReactorSpectrum.h"

namespace ana::dc {

  /**
   * @class DCLikelihood
   * @brief A class that represents the likelihood calculation for the Double Chooz experiment.
   *
   * This class inherits from the Likelihood base class and provides specific implementations
   * for calculating the likelihood based on the Double Chooz experiment data.
   */
  class DCLikelihood : public Likelihood {
   public:
    /**
     * @brief Constructs a DCLikelihood object.
     *
     * This constructor initializes the DCLikelihood object with the given options
     * and sets the number of parameters.
     *
     * @param options A shared pointer to an io::Options object that contains the configuration options.
     */
    explicit DCLikelihood(std::shared_ptr<io::Options> options);

    /**
     * @brief Default destructor for DCLikelihood class.
     */
    ~DCLikelihood() override = default;

    /**
     * @brief Calculates the likelihood based on the provided parameters.
     *
     * This function overrides the base class implementation to provide a specific
     * likelihood calculation for the Double Chooz experiment.
     *
     * @param parameter A pointer to an array of double values representing the parameters
     *                  for which the likelihood is to be calculated.
     * @return A double value representing the calculated likelihood.
     */
    [[nodiscard]] double calculate_likelihood(const double* parameter) override;

    /**
     * @brief Retrieves the measurement data for a specified detector type.
     *
     * This function returns a span of constant doubles representing the
     * measurement data associated with the given detector type.
     *
     * @param type The type of detector for which to retrieve the measurement data.
     * @return std::span<const double> A span of constant doubles containing the measurement data.
     */
    [[nodiscard]] std::span<const double> get_measurement_data(params::dc::DetectorType type) const noexcept {
      return m_MeasurementData.at(type);
    }

    [[nodiscard]] std::span<const double> get_off_off_data(params::dc::DetectorType type) const noexcept {
      return m_OffOffData.at(type);
    }

    /**
     * @brief Calculates the off-off likelihood for the given background and detector type.
     *
     * This function computes the likelihood of the off-off state based on the provided
     * background data and the specified detector type.
     *
     * @param bkg The background data used for the likelihood calculation.
     * @param type The type of detector being used (as defined in params::dc::DetectorType).
     * @return A double representing the calculated off-off likelihood.
     */
    [[nodiscard]] double calculate_off_off_likelihood(const Eigen::Array<double, 44, 1>& bkg, params::dc::DetectorType type);

    [[nodiscard]] const AccidentalBackground& accidental_background() const noexcept { return m_Accidental; }

    [[nodiscard]] const LithiumBackground& lithium_background() const noexcept { return m_Lithium; }

    [[nodiscard]] const FastNBackground& fastn_background() const noexcept { return m_FastN; }

    [[nodiscard]] const DNCBackground& dnc_background() const noexcept { return m_DNC; }

    [[nodiscard]] ReactorSpectrum& reactor_spectrum() noexcept { return m_Reactor; }

    ParameterWrapper& parameter() noexcept { return m_Parameter; }

   private:
    /**
     * @brief Calculates the default likelihood for the given parameter.
     *
     * This function calculates the likelihood for the default configuration of the
     * Double Chooz experiment based on the provided parameters.
     *
     * @param parameter The parameter for which the likelihood is to be calculated.
     * @return The calculated likelihood as a double.
     */
    [[nodiscard]] double calculate_default_likelihood(const ParameterWrapper& parameter) noexcept;

    /**
     * @brief Calculates the likelihood of the reactor split based on the given parameters.
     *
     * This function computes the likelihood of the reactor split configuration using the provided
     * parameters encapsulated in the ParameterWrapper object.
     *
     * @param parameter A constant reference to a ParameterWrapper object containing
     *                  the parameters required for the likelihood calculation.
     * @return A double representing the calculated likelihood.
     */
    // TODO
    [[nodiscard]] double calculate_reactor_split_likelihood(const ParameterWrapper& parameter) noexcept {
      return 0.0;
    }

    /**
     * @brief Recalculates the spectra based on the provided parameters.
     *
     * This function updates the spectra using the given parameter wrapper.
     *
     * @param parameter A constant reference to a ParameterWrapper object containing the parameters for recalculating the spectra.
     */
    bool recalculate_spectra(const ParameterWrapper& parameter) noexcept;

    void initialize_measurement_data();

    void read_measurement_data();

    void generate_measurement_data();

    ParameterWrapper m_Parameter;  ///< The parameter wrapper object used for likelihood calculation.

    AccidentalBackground m_Accidental;  ///< The accidental background object.
    LithiumBackground    m_Lithium;     ///< The lithium background object.
    FastNBackground      m_FastN;       ///< The fast neutron background object.
    DNCBackground        m_DNC;         ///< The delayed neutron capture background object.
    ReactorSpectrum      m_Reactor;     ///< The reactor spectrum object.

    std::vector<SpectrumBase*> m_Components;

    std::unordered_map<params::dc::DetectorType, std::array<double, 44>> m_MeasurementData;  ///< The measurement data for each detector type.
    std::unordered_map<params::dc::DetectorType, std::array<double, 44>> m_OffOffData;      ///< The off-off data for each detector type.
  };

}  // namespace ana::dc
