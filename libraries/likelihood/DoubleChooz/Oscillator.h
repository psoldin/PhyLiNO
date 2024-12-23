#pragma once

#include "Options.h"
#include "OscillationData.h"
#include "../ParameterWrapper.h"
#include "SpectrumBase.h"

namespace ana::dc {
  /**
   * @brief Represents an Oscillator.
   *
   * This class provides functionality to handle an Oscillator.
   */
  class Oscillator : public SpectrumBase {
   public:
    /**
     * @brief Constructs an Oscillator object with the given options.
     *
     * @param options The options for the Oscillator.
     */
    explicit Oscillator(std::shared_ptr<io::Options> options);

    /**
     * @brief Destructor for the Oscillator object.
     */
    ~Oscillator() override = default;

    bool check_and_recalculate(const ParameterWrapper& parameter, bool previous_calculation_step=true) noexcept override;

    /**
     * @brief Returns the calculated spectra for the given detector type.
     *
     * @param type The detector type.
     * @return The calculated spectra.
     */
    [[nodiscard]] std::span<const double> get_spectrum(params::dc::DetectorType type) const noexcept override {
      return m_Cache.at(type);
    }

   private:
    std::shared_ptr<io::Options> m_Options; /**< The options for the Oscillator. */

    using span_t = std::span<const double>;

    std::vector<OscillationData> m_CalculationData; /**< The data used for the actual computations. */

    std::unordered_map<params::dc::DetectorType, std::array<double, 80>> m_Cache; /**< The cache for the calculated spectra. */

    void add_reactor_data(const io::ReactorData& reactorData, params::dc::DetectorType type);

    void perform_cpu_oscillation(const ParameterWrapper& parameter) noexcept;

    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;
  };
}  // namespace ana::dc
