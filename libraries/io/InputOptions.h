#pragma once

// includes
#include "InputParameter.h"
#include "InputPaths.h"

namespace io {

  // TODO Documentation

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
    [[nodiscard]] int seed() const noexcept { return m_Seed; }

    /**
     * @brief Check if the program should run in silent mode.
     * 
     * @return True if the program should run in silent mode, false otherwise.
     */
    [[nodiscard]] bool silent() const noexcept { return m_Silent; }

   private:
    int  m_Seed;
    bool m_Silent;
    bool m_UseData;
    bool m_UseSystematicalErrors;
    bool m_UseStatisticalErrors;
    bool m_FakeBump;
    bool m_LikelihoodScan;

    std::string m_ConfigFile;

    std::unique_ptr<InputParameter> m_InputParameter;
    std::vector<InputPaths>         m_InputPaths;
  };
}  // namespace io
