#pragma once

#include <iostream>

namespace io {

  /**
   * @class ParameterValue
   * @brief Represents a parameter value.
   *
   * This class provides a representation of a parameter value. It is used in the I/O operations
   * to serialize and deserialize parameter values.
   */
  class ParameterValue {
  public:

    /**
     * @brief Constructs a ParameterValue object with the given value and error.
     *
     * @param value The value of the parameter.
     * @param error The error associated with the parameter value.
     */
    ParameterValue(double value, double error)
      : m_Value(value)
      , m_Error(error) { }

    /**
     * @brief Default constructor for ParameterValue class.
     * 
     * This constructor initializes a ParameterValue object with default values of -99.0 for both parameters.
     */
    ParameterValue()
      : ParameterValue(-99.0, -99.0) { }

    /**
     * Sets the value and error of the parameter.
     *
     * @param value The value of the parameter.
     * @param error The error associated with the parameter value.
     */
    void set(double value, double error) noexcept {
      m_Value = value;
      m_Error = error;
    }

    /**
     * @brief Sets the value of the parameter.
     *
     * This function sets the value of the parameter to the specified double value.
     *
     * @param value The double value to set.
     */
    void set_value(double value) noexcept {
      m_Value = value;
    }

    /**
     * @brief Sets the error value for the parameter.
     *
     * This function sets the error value for the parameter. The error value represents the uncertainty or
     * margin of error associated with the parameter value.
     *
     * @param error The error value to be set.
     */
    void set_error(double error) noexcept {
      m_Error = error;
    }

    /**
     * @brief Get the value of the parameter.
     * 
     * @return The value of the parameter as a double.
     */
    [[nodiscard]] inline double value() const noexcept { return m_Value; }

    /**
     * @brief Get the error associated with the parameter value.
     * 
     * This function returns the error value associated with the parameter value.
     * 
     * @return The error value of the parameter.
     */
    [[nodiscard]] inline double error() const noexcept { return m_Error; }

    /**
     * @brief Overloaded stream insertion operator for ParameterValue objects.
     *
     * This function allows a ParameterValue object to be inserted into an output stream.
     * The parameter value and error are formatted and written to the stream.
     *
     * @param os The output stream.
     * @param value The ParameterValue object to be inserted.
     * @return The modified output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const ParameterValue& value) noexcept {
      os << '[' << value.value() << ',' << value.error() << ']';
      return os;
    }

  private:
    double m_Value; // The value of the parameter
    double m_Error; // The error associated with the parameter value
  };

} // namespace io
