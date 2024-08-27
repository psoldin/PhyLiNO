#pragma once

#include "Parameter.h"
#include "ParameterWrapper.h"

// STL includes
#include <array>
#include <span>
#include <vector>

namespace ana::dc {

  /**
   * @brief Wrapper class for a parameter array.
   *
   * The ParameterWrapper class provides a convenient way to access and manipulate a parameter array.
   * It wraps a raw double pointer and provides various member functions for accessing the parameters.
   */
  class DCParameterWrapper : public ParameterWrapper {
   public:
    /**
     * @brief Constructs a ParameterWrapper object with a raw double pointer to the parameter array.
     *
     * @param parameter A pointer to the parameter array.
     */
    explicit DCParameterWrapper(const double* parameter);

    /**
     * @brief Default destructor.
     */
    ~DCParameterWrapper() final = default;

    /**
     * @brief Resets the parameter to the given values.
     *
     * @param parameter A pointer to the new value of the parameters.
     */
    void reset_parameter(const double* parameter);

    /**
     * @brief Returns the value of the parameter with the given index.
     *
     * @param index The index of the parameter.
     * @return The value of the parameter.
     */
    [[nodiscard]] double operator[](std::size_t index) const noexcept { return m_UnifiedParameters[index]; }

    /**
     * @brief Returns a span of the raw parameters.
     *
     * This function returns a std::span object that provides a view of the raw parameters.
     *
     * @return A std::span object containing the raw parameters.
     */
    [[nodiscard]] std::span<const double> raw_parameters() const noexcept {
        return {m_RawParameter, params::number_of_parameters()};
    }

    /**
     * @brief Returns an iterator to the beginning of the unified parameters.
     *
     * @return An iterator to the beginning of the unified parameters.
     */
    [[nodiscard]] inline auto begin() const noexcept { return m_UnifiedParameters.begin(); }

    /**
     * @brief Returns an iterator to the end of the unified parameters.
     *
     * @return An iterator to the end of the unified parameters.
     */
    [[nodiscard]] inline auto end() const noexcept { return m_UnifiedParameters.end(); }

    /**
     * @brief Returns a reverse iterator to the beginning of the unified parameters.
     *
     * @return A reverse iterator to the beginning of the unified parameters.
     */
    [[nodiscard]] inline auto rbegin() const noexcept { return m_UnifiedParameters.rbegin(); }

    /**
     * @brief Returns a reverse iterator to the end of the unified parameters.
     *
     * @return A reverse iterator to the end of the unified parameters.
     */
    [[nodiscard]] inline auto rend() const noexcept { return m_UnifiedParameters.rend(); }

    /**
     * @brief Returns a constant iterator to the beginning of the unified parameters.
     *
     * @return A constant iterator to the beginning of the unified parameters.
     */
    [[nodiscard]] inline auto cbegin() const noexcept { return m_UnifiedParameters.cbegin(); }

    /**
     * @brief Returns a constant iterator to the end of the unified parameters.
     *
     * @return A constant iterator to the end of the unified parameters.
     */
    [[nodiscard]] inline auto cend() const noexcept { return m_UnifiedParameters.cend(); }

    /**
     * @brief Returns a constant reverse iterator to the beginning of the unified parameters.
     *
     * @return A constant reverse iterator to the beginning of the unified parameters.
     */
    [[nodiscard]] inline auto crbegin() const noexcept { return m_UnifiedParameters.crbegin(); }

    /**
     * @brief Returns a constant reverse iterator to the end of the unified parameters.
     *
     * @return A constant reverse iterator to the end of the unified parameters.
     */
    [[nodiscard]] inline auto crend() const noexcept { return m_UnifiedParameters.crend(); }

   private:
    std::array<double, params::number_of_parameters()> m_UnifiedParameters;  // Unified parameters array

    /**
     * @brief Unifies the parameters.
     *
     * This function unifies the parameters
     */
    void unify_parameters();
  };

}  // namespace ana