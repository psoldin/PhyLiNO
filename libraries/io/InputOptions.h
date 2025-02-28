#pragma once

// STL includes
#include <map>

// includes
#include "DoubleChooz/DCInputOptions.h"
#include "InputOptionBase.h"
#include "InputParameter.h"

namespace io {

  /**
   * @brief Class representing input options for the program.
   *
   * This class provides methods to access and manipulate the input options
   * for the program. It stores information such as the command line arguments,
   * random seed, silent mode flag, and other related options.
   */
  class InputOptions {
   public:
    /**
     * @brief Constructor that initializes the input options.
     *
     * @param argc The number of command line arguments.
     * @param argv The array of command line arguments.
     */
    InputOptions(int argc, char** argv);

    /** Default destructor */
    ~InputOptions() = default;

    /**
     * @brief Get the global random seed.
     *
     * @return The global random seed for number generation.
     */
    [[nodiscard]] long seed() const noexcept { return m_Seed; }

    /**
     * @brief Check if the program should run in silent mode.
     *
     * @return True if the program should run in silent mode, false otherwise.
     */
    [[nodiscard]] bool silent() const noexcept { return m_Silent; }

    [[nodiscard]] const auto& input_parameters() const noexcept { return *m_InputParameter; }

    [[nodiscard]] const auto& double_chooz() const noexcept { return *m_DCInputOptions; }

    [[nodiscard]] bool use_multi_threading() const noexcept { return m_MultiThreadingCores > 1; }

    [[nodiscard]] int multi_threading_cores() const noexcept { return m_MultiThreadingCores; }

    [[nodiscard]] const boost::property_tree::ptree& config_tree() const noexcept { return m_ConfigTree; }

    [[nodiscard]] double tolerance() const noexcept { return m_Tolerance; }

   private:
    long m_Seed;                /**< The global random seed. */
    bool m_Silent;              /**< Flag indicating if the program should run in silent mode. */
    int  m_MultiThreadingCores; /**< The number of cores to use for multi-threading. */
    double m_Tolerance;         /**< The tolerance for the minimizer. */

    std::string m_ConfigFile; /**< The configuration file path. */

    boost::property_tree::ptree m_ConfigTree;  // < The configuration tree

    std::shared_ptr<InputParameter> m_InputParameter; /**< The input parameter object. */

    std::shared_ptr<io::dc::DCInputOptions> m_DCInputOptions; /**< The Double Chooz input options. */
  };
}  // namespace io
