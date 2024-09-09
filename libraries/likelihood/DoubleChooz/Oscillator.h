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

    void recalculate_spectra(const ParameterWrapper& parameter, std::vector<Eigen::Array<double, 80, 1>>& spectra) const noexcept;

   private:
    std::shared_ptr<io::Options> m_Options; /**< The options for the Oscillator. */

    using span_t = std::span<const double>;

    std::vector<OscillationData> m_CalculationData; /**< The data used for the actual computations. */

    void add_reactor_data(const io::ReactorData& reactorData, params::dc::DetectorType type);
  };
}  // namespace ana::dc
