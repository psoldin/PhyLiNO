#pragma once
#include "../InputOptionBase.h"
#include "DCDetectorPaths.h"
#include "Parameter.h"

namespace io::dc {
  class DCDetectorPaths;

  /**
   * @class DCInputOptions
   * @brief A class for setting Double Chooz specific input parameters.
   *
   * The DCInputOptions class is responsible for initializing and managing
   * input options specific to the Double Chooz experiment. It provides
   * methods to read and process input options from a Boost program options
   * variables map, and to check the status of various options.
   */
  class DCInputOptions : public InputOptionBase {
   public:
    /**
     * @brief Constructor for the DCInputOptions class.
     *
     * This constructor initializes an instance of the DCInputOptions class.
     */
    DCInputOptions();

    /**
     * Default Destructor for the DCInputOptions class.
     */
    ~DCInputOptions() final = default;

    /**
     * @brief Reads and processes input options from the provided variables map.
     *
     * This function overrides a virtual function to read and handle input options
     * specified in the given Boost program options variables map.
     *
     * @param vm A constant reference to a Boost program options variables map
     *           containing the input options to be read.
     */
    void read(const boost::program_options::variables_map& vm, const boost::property_tree::ptree& config) final;

    /**
     * @brief Checks if data usage is enabled.
     *
     * This function returns a boolean value indicating whether the use of data is enabled.
     *
     * @return true if data usage is enabled, false otherwise.
     */
    [[nodiscard]] bool use_data() const noexcept { return m_UseData; }

    /**
     * @brief Checks if statistical errors are used.
     *
     * This function returns a boolean value indicating whether statistical errors
     * are being used in the current Asimov context.
     *
     * @return true if statistical errors are used, false otherwise.
     */
    [[nodiscard]] bool use_statistical_errors() const noexcept { return m_UseStatisticalErrors; }

    /**
     * @brief Checks if systematical errors are used.
     *
     * This function returns a boolean value indicating whether systematical errors
     * are being used in the current Asimov context.
     *
     * @return true if systematical errors are used, false otherwise.
     */
    [[nodiscard]] bool use_systematic_errors() const noexcept { return m_UseSystematicErrors; }

    /**
     * @brief Checks if the fake bump option is enabled.
     *
     * This function returns the status of the fake bump option.
     *
     * @return true if the fake bump option is enabled, false otherwise.
     */
    [[nodiscard]] bool fake_bump() const noexcept { return m_FakeBump; }

    /**
     * @brief Checks if the likelihood scan option is enabled.
     *
     * This function returns the status of the likelihood scan option.
     *
     * @return true if the likelihood scan option is enabled, false otherwise.
     */
    [[nodiscard]] bool likelihood_scan() const noexcept { return m_LikelihoodScan; }

    /**
     * @brief Checks if the sterile option is enabled.
     *
     * This function returns whether the sterile option is enabled or not.
     *
     * @return true if the sterile option is enabled, false otherwise.
     */
    [[nodiscard]] bool use_sterile() const noexcept { return m_UseSterile; }

    /**
     * @brief Checks if the reactor split option is enabled.
     *
     * This function returns a boolean value indicating whether the reactor split
     * option is enabled or not.
     * @return true if the reactor split option is enabled, false otherwise.
     */
    [[nodiscard]] bool reactor_split() const noexcept { return m_ReactorSplit; }

    [[nodiscard]] const auto& input_paths(params::dc::DetectorType type) const noexcept { return m_InputPaths.at(type); }

    [[nodiscard]] const std::string& config_file_path() const noexcept { return m_ConfigFile; }

   private:
    /**
     * @brief Sets up the input options for the application.
     *
     * This function initializes and configures the necessary input options
     * required for the application to run properly.
     */
    void setup_options();

    std::unordered_map<params::dc::DetectorType, DCDetectorPaths> m_InputPaths;  // < The input paths for the Double Chooz experiment

    std::string m_ConfigFile;  // < The configuration file path

    bool m_UseData;               // < Use Double Chooz Measurement Data
    bool m_UseStatisticalErrors;  // < Use Statistics Errors for toy-Spectra creation
    bool m_UseSystematicErrors;   // < Use Systematic Errors for toy-Spectra creation
    bool m_FakeBump;              // < Add fake bump to fake data
    bool m_LikelihoodScan;        // < Perform a likelihood scan
    bool m_UseSterile;            // < Use Sterile Neutrino Parameters
    bool m_ReactorSplit;          // < Use reactor split
  };
}  // namespace io::dc