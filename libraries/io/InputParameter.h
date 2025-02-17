#pragma once

// STL includes
#include <string>
#include <iostream>

// boost includes
#include <boost/property_tree/ptree.hpp>

namespace io {

  /**
   * @class InputParameter
   * @brief Represents a set of input parameters.
   *
   * The InputParameter class provides methods to access input parameters from the config file.
   * Each input parameter has a name, value, uncertainty, fixed status, and constrained status.
   * The class also provides methods to retrieve information about the parameters, such as the number of parameters,
   * the names of all parameters, the parameters themselves, and the fixed and constrained status of all parameters.
   */
  class InputParameter {
    class Parameter;
   public:
    /**
     * @brief Constructor for InputParameter.
     *
     * @param tree The boost property tree containing the input parameters.
     */
    explicit InputParameter(const boost::property_tree::ptree& tree) {
      try {
        for (const auto& [_, parameter] : tree) {
          m_Parameters.emplace_back(parameter);
          m_Fixed.emplace_back(parameter.get<bool>("fixed"));
          m_Constrained.emplace_back(parameter.get<bool>("constrained"));
          m_Names.emplace_back(parameter.get<std::string>("name"));
        }
      } catch (std::exception& e) {
        std::cout << "A problem occurred in InputParameters class: " << e.what() << '\n';
        throw;
      }
    }

    /**
     * @brief Get the name of the parameter at the specified index.
     *
     * @param i The index of the parameter.
     * @return The name of the parameter.
     */
    [[nodiscard]] const std::string& name(int i) const noexcept { return m_Names[i]; }

    /**
     * @brief Get the value of the parameter at the specified index.
     *
     * @param i The index of the parameter.
     * @return The value of the parameter.
     */
    [[nodiscard]] double value(int i) const noexcept { return m_Parameters[i].value(); }

    /**
     * @brief Get the uncertainty of the parameter at the specified index.
     *
     * @param i The index of the parameter.
     * @return The uncertainty of the parameter.
     */
    [[nodiscard]] double uncertainty(int i) const noexcept { return m_Parameters[i].uncertainty(); }

    /**
     * @brief Check if the parameter at the specified index is fixed.
     *
     * @param i The index of the parameter.
     * @return True if the parameter is fixed, false otherwise.
     */
    [[nodiscard]] bool fixed(int i) const noexcept { return m_Fixed[i]; }

    /**
     * @brief Check if the parameter at the specified index is constrained.
     *
     * @param i The index of the parameter.
     * @return True if the parameter is constrained, false otherwise.
     */
    [[nodiscard]] bool constrained(int i) const noexcept { return m_Constrained[i]; }

    /**
     * @brief Get the number of parameters.
     *
     * @return The number of parameters.
     */
    [[nodiscard]] std::size_t size() const noexcept { return m_Parameters.size(); }

    /**
     * @brief Get the names of all parameters.
     *
     * @return The names of all parameters.
     */
    [[nodiscard]] const std::vector<std::string>& names() const noexcept { return m_Names; }

    /**
     * @brief Get the parameters.
     *
     * @return The parameters.
     */
    [[nodiscard]] const std::vector<Parameter>& parameters() const noexcept { return m_Parameters; }

    /**
     * @brief Get the fixed status of all parameters.
     *
     * @return The fixed status of all parameters.
     */
    [[nodiscard]] const std::vector<bool>& fixed() const noexcept { return m_Fixed; }

    /**
     * @brief Get the constrained status of all parameters.
     *
     * @return The constrained status of all parameters.
     */
    [[nodiscard]] const std::vector<bool>& constrained() const noexcept { return m_Constrained; }

   private:
    std::vector<Parameter>   m_Parameters;   ///< The parameters.
    std::vector<bool>        m_Fixed;        ///< The fixed status of the parameters.
    std::vector<bool>        m_Constrained;  ///< The constrained status of the parameters.
    std::vector<std::string> m_Names;        ///< The names of the parameters.

    /**
     * @brief Constructs an InputParameter object.
     *
     * @param parameter The boost::property_tree::ptree object containing the parameter information.
     */
    class Parameter {
     public:
      /**
       * @brief Constructs a Parameter object.
       *
       * @param parameter The boost::property_tree::ptree object containing the parameter information.
       */
      explicit Parameter(const boost::property_tree::ptree& parameter)
        : m_Value(parameter.get<double>("start"))
        , m_Uncertainty(parameter.get<double>("uncertainty")) {
      }

      /**
       * @brief Gets the value of the parameter.
       *
       * @return The value of the parameter.
       */
      [[nodiscard]] double value() const noexcept { return m_Value; }

      /**
       * @brief Gets the uncertainty of the parameter.
       *
       * @return The uncertainty of the parameter.
       */
      [[nodiscard]] double uncertainty() const noexcept { return m_Uncertainty; }

     private:
      double m_Value;        ///< The value of the parameter.
      double m_Uncertainty;  ///< The uncertainty of the parameter.
    };
  };

}  // namespace io
