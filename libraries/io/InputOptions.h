#pragma once

// STL includes
#include <map>

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

    /**
     * @brief Check if the program should use data.
     *
     * @return True if the program should use data, false otherwise.
     */
    [[nodiscard]] bool use_data() const noexcept { return m_UseData; }

    /**
     * @brief Check if the program should use statistical errors.
     *
     * @return True if the program should use statistical errors, false otherwise.
     */
    [[nodiscard]] bool use_statistical_errors() const noexcept { return m_UseStatisticalErrors; }

    /**
     * @brief Check if the program should use systematical errors.
     *
     * @return True if the program should use systematical errors, false otherwise.
     */
    [[nodiscard]] bool use_systematical_errors() const noexcept { return m_UseSystematicalErrors; }

    /**
     * @brief Check if the program should use fake bump.
     *
     * @return True if the program should use fake bump, false otherwise.
     */
    [[nodiscard]] bool fake_bump() const noexcept { return m_FakeBump; }

    /**
     * @brief Check if the program should perform likelihood scan.
     *
     * @return True if the program should perform likelihood scan, false otherwise.
     */
    [[nodiscard]] bool likelihood_scan() const noexcept { return m_LikelihoodScan; }

    /**
     * @brief Check if the program should use sterile.
     *
     * @return True if the program should use sterile, false otherwise.
     */
    [[nodiscard]] bool use_sterile() const noexcept { return m_UseSterile; }

    [[nodiscard]] const auto& input_parameter() const noexcept { return *m_InputParameter; }

    [[nodiscard]] const auto& input_paths(params::dc::DetectorType type) const noexcept { return m_InputPaths.at(type); }

    [[nodiscard]] bool use_reactor_split() const noexcept;

   private:
    long m_Seed;                  /**< The global random seed. */
    bool m_Silent;                /**< Flag indicating if the program should run in silent mode. */
    bool m_UseData;               /**< Flag indicating if the program should use data. */
    bool m_UseSystematicalErrors; /**< Flag indicating if the program should use systematical errors. */
    bool m_UseStatisticalErrors;  /**< Flag indicating if the program should use statistical errors. */
    bool m_FakeBump;              /**< Flag indicating if the program should use a fake bump. */
    bool m_LikelihoodScan;        /**< Flag indicating if the program should perform a likelihood scan. */
    bool m_UseSterile;            /**< Flag indicating if the program should use sterile neutrino parameters. */

    std::string m_ConfigFile; /**< The configuration file path. */

    std::unique_ptr<InputParameter>                m_InputParameter; /**< The input parameter object. */
    std::map<params::dc::DetectorType, InputPaths> m_InputPaths;     /**< The input paths map. */
  };
}  // namespace io
