  #pragma once

#include "../io/Parameter.h"

  // STL includes
#include <array>
#include <span>
#include <vector>

#include "Options.h"

  namespace ana::dc { // TODO move class to ana namespace

  /**
   * @brief Wrapper class for a parameter array.
   *
   * The ParameterWrapper class provides a convenient way to access and manipulate a parameter array.
   * It wraps a raw double pointer and provides various member functions for accessing the parameters.
   */
  class ParameterWrapper {
    using transform_fn_t = void(*)(const io::Options& options, std::span<double> parameter);
   public:
/**
     * @brief Constructs a ParameterWrapper object.
     *
     * @param nParameter The number of parameters.
     * @param transform_fn A function pointer to transform the parameters (default is nullptr).
     */
    ParameterWrapper(std::size_t nParameter, std::shared_ptr<io::Options> options, transform_fn_t transform_fn = nullptr);
    /**
     * @brief Default destructor.
     */
    ~ParameterWrapper() = default;

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
    [[nodiscard]] double operator[](int index) const noexcept { return m_CurrentParameters[index]; }

    [[nodiscard]] std::span<const double> sub_range(int start, int end) const noexcept {
      return std::span(m_CurrentParameters).subspan(start, end - start);
    }

    /**
     * @brief Returns a span of the raw parameters.
     *
     * This function returns a std::span object that provides a view of the raw parameters.
     *
     * @return A std::span object containing the raw parameters.
     */
    [[nodiscard]] std::span<const double> raw_parameters() const noexcept {
      return {m_RawParameter, m_CurrentParameters.size()};
    }

    /**
     * @brief Returns an iterator to the beginning of the unified parameters.
     *
     * @return An iterator to the beginning of the unified parameters.
     */
    [[nodiscard]] auto begin() const noexcept { return m_CurrentParameters.begin(); }

    /**
     * @brief Returns an iterator to the end of the unified parameters.
     *
     * @return An iterator to the end of the unified parameters.
     */
    [[nodiscard]] auto end() const noexcept { return m_CurrentParameters.end(); }

    /**
     * @brief Returns a reverse iterator to the beginning of the unified parameters.
     *
     * @return A reverse iterator to the beginning of the unified parameters.
     */
    [[nodiscard]] auto rbegin() const noexcept { return m_CurrentParameters.rbegin(); }

    /**
     * @brief Returns a reverse iterator to the end of the unified parameters.
     *
     * @return A reverse iterator to the end of the unified parameters.
     */
    [[nodiscard]] auto rend() const noexcept { return m_CurrentParameters.rend(); }

    /**
     * @brief Returns a constant iterator to the beginning of the unified parameters.
     *
     * @return A constant iterator to the beginning of the unified parameters.
     */
    [[nodiscard]] auto cbegin() const noexcept { return m_CurrentParameters.cbegin(); }

    /**
     * @brief Returns a constant iterator to the end of the unified parameters.
     *
     * @return A constant iterator to the end of the unified parameters.
     */
    [[nodiscard]] auto cend() const noexcept { return m_CurrentParameters.cend(); }

    /**
     * @brief Returns a constant reverse iterator to the beginning of the unified parameters.
     *
     * @return A constant reverse iterator to the beginning of the unified parameters.
     */
    [[nodiscard]] auto crbegin() const noexcept { return m_CurrentParameters.crbegin(); }

    /**
     * @brief Returns a constant reverse iterator to the end of the unified parameters.
     *
     * @return A constant reverse iterator to the end of the unified parameters.
     */
    [[nodiscard]] auto crend() const noexcept { return m_CurrentParameters.crend(); }

    /**
     * @brief Checks if the parameter at the given index has changed.
     *
     * This function compares the parameter at the specified index in the current parameter array
     * with the corresponding parameter in the previous parameter array.
     *
     * @param idx The index of the parameter to check.
     * @return True if the parameter has changed, false otherwise.
     */
    [[nodiscard]] bool check_parameter_changed(int idx) const;

    /**
     * @brief Checks if any parameter in the given range has changed.
     *
     * This function compares the parameters in the specified range in the current parameter array
     * with the corresponding parameters in the previous parameter array.
     *
     * @param from The starting index of the range.
     * @param to The ending index of the range.
     * @return True if any parameter in the range has changed, false otherwise.
     */
    [[nodiscard]] bool check_parameter_changed(int from, int to) const;

   private:
    std::vector<double> m_CurrentParameters;  // Unified parameters array
    std::vector<double> m_PreviousParameters;  // Previous parameter set for comparison
    std::vector<char> m_ParameterChanged;  // Array to store the changed parameters
    std::size_t m_NParameter;  // Number of parameters
    std::shared_ptr<io::Options> m_Options;  // Options object
    const double* m_RawParameter;  // Pointer to the raw parameter array
    transform_fn_t m_TransformFn;  // Function to transform the parameters

    /**
     * @brief Unifies the parameters.
     *
     * This function unifies the parameters
     */
    void unify_parameters();
  };

}  // namespace ana::dc
