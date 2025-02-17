#pragma once

#include "../Likelihood.h"
#include "ParameterWrapper.h"
#include "Options.h"

#include "AccidentalBackground.h"
#include "LithiumBackground.h"
#include "FastNBackground.h"
#include "DNCBackground.h"
#include "ReactorSpectrum.h"

namespace ana::dc {

  inline void correlate_parameters(const io::Options& options, std::span<double> parameters) {
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;
    using namespace params;

    // FDI and FDII lithium background rates are fully correlated
    parameters[index(FDI, BkgRLi)] = parameters[index(FDII, BkgRLi)];

    Eigen::Vector<double, 7> correlated_energy = {parameters[EnergyA],
                                              parameters[index(FDI,  EnergyB)],
                                              parameters[index(ND,   EnergyB)],
                                              parameters[index(FDII, EnergyB)],
                                              parameters[index(FDI,  EnergyC)],
                                              parameters[index(ND,   EnergyC)],
                                              parameters[index(FDII, EnergyC)]};

    const auto& covEigVecMatrix = options.starting_parameter().correlations().energy_correlation_matrix();


  }
  /*
  void correlate_parameters(const io::Options& options, std::span<double> parameters) {
    using enum params::dc::DetectorType;
    using enum params::dc::Detector;
    using namespace params;

    // FDI and FDII lithium background rates are fully correlated
    parameters[index(FDI, BkgRLi)] = parameters[index(FDII, BkgRLi)];

    Eigen::Vector<double, 7> correlated_energy = {parameters[EnergyA],
                                                  parameters[index(FDI,  EnergyB)],
                                                  parameters[index(ND,   EnergyB)],
                                                  parameters[index(FDII, EnergyB)],
                                                  parameters[index(FDI,  EnergyC)],
                                                  parameters[index(ND,   EnergyC)],
                                                  parameters[index(FDII, EnergyC)]};

    const auto& covEigVecMatrix = options.starting_parameter().correlations().energy_correlation_matrix();

    correlated_energy = covEigVecMatrix * correlated_energy;

    parameters[EnergyA] = correlated_energy[0];
    parameters[index(FDI,  EnergyB)] = correlated_energy[1];
    parameters[index(ND,   EnergyB)] = correlated_energy[2];
    parameters[index(FDII, EnergyB)] = correlated_energy[3];
    parameters[index(FDI,  EnergyC)] = correlated_energy[4];
    parameters[index(ND,   EnergyC)] = correlated_energy[5];
    parameters[index(FDII, EnergyC)] = correlated_energy[6];
  }
  */

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
    explicit DCLikelihood(std::shared_ptr<io::Options> options)
      : Likelihood(std::move(options))
      , m_Parameter(params::number_of_parameters(), m_Options)
      , m_Accidental(m_Options)
      , m_Lithium(m_Options)
      , m_FastN(m_Options)
      , m_DNC(m_Options)
      , m_Reactor(m_Options) {}

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
    // TODO
    [[nodiscard]] double calculate_off_off_likelihood(std::span<const double> bkg, params::dc::DetectorType type) { return 0.0; }

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
    [[nodiscard]] double calculate_reactor_split_likelihood(const ParameterWrapper& parameter) noexcept;

    /**
     * @brief Recalculates the spectra based on the provided parameters.
     *
     * This function updates the spectra using the given parameter wrapper.
     *
     * @param parameter A constant reference to a ParameterWrapper object containing the parameters for recalculating the spectra.
     */
    bool recalculate_spectra(const ParameterWrapper& parameter) noexcept;

    ParameterWrapper m_Parameter; ///< The parameter wrapper object used for likelihood calculation.

    AccidentalBackground m_Accidental; ///< The accidental background object.
    LithiumBackground m_Lithium;       ///< The lithium background object.
    FastNBackground m_FastN;           ///< The fast neutron background object.
    DNCBackground m_DNC;               ///< The delayed neutron capture background object.
    ReactorSpectrum m_Reactor;         ///< The reactor spectrum object.

    std::unordered_map<params::dc::DetectorType, std::array<double, 44>> m_MeasurementData; ///< The measurement data for each detector type.

    void correlate_parameters(std::span<double> parameters) noexcept { }
  };

}  // namespace ana::dc
