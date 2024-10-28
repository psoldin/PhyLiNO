#pragma once

// includes
#include "InputOptions.h"
#include "ParameterValue.h"
#include "Parameter.h"

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
       * @brief Returns a span of parameter values.
       *
       * This function returns a `std::span` of `ParameterValue` objects, representing the parameter values.
       * The `std::span` allows for efficient and safe access to the underlying data.
       *
       * @return A `std::span` of `ParameterValue` objects.
       */
      [[nodiscard]] std::span<const ParameterValue> parameterValues() const noexcept { return m_ParameterValues; }


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

      [[nodiscard]] ParameterValue energy_correction_parameter_B(params::dc::DetectorType type) const noexcept { return m_EnergyCorrectionParameterB.at(params::dc::cast_to_base_type(type)); }

      [[nodiscard]] ParameterValue energy_correction_parameter_C(params::dc::DetectorType type) const noexcept { return m_EnergyCorrectionParameterC.at(params::dc::cast_to_base_type(type)); }
    private:
      std::vector<ParameterValue> m_ParameterValues;

      std::array<double, params::number_of_data_sets()> m_OnLifeTime;

      ParameterValue m_EnergyCorrectionParameterA;
      std::unordered_map<params::dc::DetectorType, ParameterValue> m_EnergyCorrectionParameterB;
      std::unordered_map<params::dc::DetectorType, ParameterValue> m_EnergyCorrectionParameterC;
  };
}
