#pragma once

#include "Options.h"
#include "OscillationData.h"
#include "ParameterWrapper.h"

namespace ana::dc {
  /**
   * @brief Represents an Oscillator.
   *
   * This class provides functionality to handle an Oscillator.
   */
  class Oscillator {
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
    ~Oscillator() = default;

    void recalculate_spectra(const ParameterWrapper& parameter) noexcept;

    /**
     * @brief Returns the calculated spectra for the given detector type.
     *
     * @param type The detector type.
     * @return The calculated spectra.
     */
    [[nodiscard]] const Eigen::Array<double, 80, 1>& get_spectrum(params::dc::DetectorType type) const noexcept {
      return m_Cache.at(type);
    }

   private:
    std::shared_ptr<io::Options> m_Options; /**< The options for the Oscillator. */

    using span_t = std::span<const double>;

    std::vector<OscillationData> m_CalculationData; /**< The data used for the actual computations. */

    std::unordered_map<params::dc::DetectorType, Eigen::Array<double, 80, 1>> m_Cache; /**< The cache for the calculated spectra. */

    void add_reactor_data(const io::ReactorData& reactorData, params::dc::DetectorType type);

    void perform_cpu_oscillation(const ParameterWrapper& parameter) noexcept;
  };
}  // namespace ana::dc
