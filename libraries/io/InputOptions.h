#pragma once

// STL includes
#include <map>

// includes
#include "InputOptionBase.h"
#include "InputParameter.h"
#include "DoubleChooz/InputPaths.h"
#include "DoubleChooz/DCInputOptions.h"

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

    [[nodiscard]] const auto& input_parameter() const noexcept { return *m_InputParameter; }

    [[nodiscard]] const auto& input_paths(params::dc::DetectorType type) const noexcept { return m_InputPaths.at(type); }

    [[nodiscard]] const auto& double_chooz() const noexcept { return *m_DCInputOptions; }

    [[nodiscard]] bool use_multi_threading() const noexcept { return m_MultiThreadingCores > 1; }

    [[nodiscard]] int multi_threading_cores() const noexcept { return m_MultiThreadingCores; }

   private:
    long m_Seed;                  /**< The global random seed. */
    bool m_Silent;                /**< Flag indicating if the program should run in silent mode. */
    int m_MultiThreadingCores;    /**< The number of cores to use for multi-threading. */

    std::string m_ConfigFile; /**< The configuration file path. */

    std::unique_ptr<InputParameter>                m_InputParameter; /**< The input parameter object. */
    std::map<params::dc::DetectorType, dc::InputPaths> m_InputPaths;     /**< The input paths map. */

    std::unique_ptr<dc::DCInputOptions> m_DCInputOptions; /**< The Double Chooz input options. */
  };
}  // namespace io
