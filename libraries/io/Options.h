#pragma once

#include "DataBase.h"
#include "InputOptions.h"

namespace io {

  /**
   * Main class for the options that are handled in the Likelihood fit.
   * Here the command line arguments as well as the measurement data are accessible.
   * 
  */
  class Options {
   public:
    /**
     * Constructor
     * @param argc Command line argc
     * @param argv Command line argv
     */
    Options(int argc, char** argv);

    /** Default constructor */
    Options()
      : Options(1, nullptr) {}

    /** Default destructor */
    ~Options() = default;

    /**
     * @brief Get the input options.
     *
     * This function returns a reference to the `InputOptions` object containing the input options.
     *
     * @return A const reference to the `InputOptions` object.
     */
    [[nodiscard]] const InputOptions& inputOptions() const noexcept { return m_InputOptions; }

    /**
     * @brief Returns a reference to the underlying database.
     *
     * This function returns a const reference to the `DataBase` object associated with the `Options` class.
     * The `DataBase` object contains the data used by the `Options` class for its operations.
     *
     * @return A const reference to the `DataBase` object.
     */
    [[nodiscard]] const DataBase& dataBase() const noexcept { return m_DataBase; }

  private:
    InputOptions m_InputOptions;
    DataBase     m_DataBase;
  };

  

}  // namespace io