#pragma once

// includes
#include "../InputOptions.h"
#include "../Parameter.h"
#include "../ParameterValue.h"

// Eigen includes
#include <Eigen/Dense>

// STL includes
#include <array>
#include <span>
#include <vector>

namespace io {

  class StartingParameter {
   public:
    explicit StartingParameter(const InputOptions& inputOptions);

    ~StartingParameter() = default;

    /**
     * @brief Get the on lifetime for a specific detector type.
     *
     * This function returns the on lifetime value for a given detector type.
     *
     * @param type The detector type.
     * @return The on lifetime value.
     */
    [[nodiscard]] double on_lifetime(params::dc::DetectorType type) const noexcept { return m_OnLifeTime[params::get_index(type)]; }

    [[nodiscard]] ParameterValue energy_correction_parameter_A() const noexcept { return m_EnergyCorrectionParameterA; }

    [[nodiscard]] ParameterValue energy_correction_parameter_B(params::dc::DetectorType type) const noexcept { return m_EnergyCorrectionParameterB.at(cast_to_base_type(type)); }

    [[nodiscard]] ParameterValue energy_correction_parameter_C(params::dc::DetectorType type) const noexcept { return m_EnergyCorrectionParameterC.at(cast_to_base_type(type)); }

   private:
    const InputOptions&         m_InputOptions;

    std::array<double, params::number_of_data_sets()> m_OnLifeTime;

    ParameterValue                                               m_EnergyCorrectionParameterA;
    std::unordered_map<params::dc::DetectorType, ParameterValue> m_EnergyCorrectionParameterB;
    std::unordered_map<params::dc::DetectorType, ParameterValue> m_EnergyCorrectionParameterC;
  };
}  // namespace io
