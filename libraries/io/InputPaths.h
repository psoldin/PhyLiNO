#pragma once

// includes
#include "Parameter.h"

// STL includes
#include <string>

// boost includes
#include <boost/program_options/options_description.hpp>

namespace io {

  /**
   * @brief Class for handling input paths for the PhyLiNO library.
   * 
   * This class provides methods for accessing various input paths required by the PhyLiNO library.
   * These include paths for detector data, reactor neutrino data, covariance matrices, and background data.
   * 
   * @note This class is not copyable or movable.
   */
  /**
   * @brief Class for managing input paths for the detector data and background simulations.
   */
  class InputPaths {
   public:
    /**
     * @brief Constructor for InputPaths class.
     * @param section The detector section name.
     */
    InputPaths(std::string&& section)
      : m_DetectorSection(std::move(section)) {}

    ~InputPaths() = default;

    /**
     * @brief Getter function for detector section name.
     * @return The detector section name.
     */
    [[nodiscard]] const std::string& detector_section() const noexcept { return m_DetectorSection; }

    /**
     * @brief Getter function for data path.
     * @return The data path.
     */
    [[nodiscard]] const std::string& data_path() const noexcept { return m_DataPath; }

    /**
     * @brief Getter function for off-off data path.
     * @return The off-off data path.
     */
    [[nodiscard]] const std::string& data_offoff_path() const noexcept { return m_OffOffDataPath; }

    /**
     * @brief Getter function for reactor neutrino data path.
     * @return The reactor neutrino data path.
     */
    [[nodiscard]] const std::string& reactor_neutrino_data_path() const noexcept { return m_ReactorPath; }

    /**
     * @brief Getter function for reactor neutrino tree name.
     * @return The reactor neutrino tree name.
     */
    [[nodiscard]] const std::string& reactor_neutrino_tree_name() const noexcept { return m_ReactorTree; }

    /**
     * @brief Getter function for neutrino visual energy branch name.
     * @return The neutrino visual energy branch name.
     */
    [[nodiscard]] const std::string& neutrino_visual_energy_branch() const noexcept { return m_ReactorVisualEnergyName; }

    /**
     * @brief Getter function for neutrino true energy branch name.
     * @return The neutrino true energy branch name.
     */
    [[nodiscard]] const std::string& neutrino_true_energy_branch() const noexcept { return m_ReactorTrueEnergyName; }

    /**
     * @brief Getter function for neutrino distance branch name.
     * @return The neutrino distance branch name.
     */
    [[nodiscard]] const std::string& neutrino_distance_branch() const noexcept { return m_ReactorDistanceName; }

    /**
     * @brief Getter function for neutrino GDML branch name.
     * @return The neutrino GDML branch name.
     */
    [[nodiscard]] const std::string& neutrino_GDML_branch() const noexcept { return m_ReactorGDMLName; }

    /**
     * @brief Getter function for covariance matrix path.
     * @param type The background type.
     * @return The covariance matrix path.
     */
    [[nodiscard]] const std::string& covarianceMatrix_path(params::BackgroundType type) const;

    /**
     * @brief Getter function for covariance matrix name.
     * @param type The background type.
     * @return The covariance matrix name.
     */
    [[nodiscard]] const std::string& covarianceMatrix_name(params::BackgroundType type) const;

    /**
     * @brief Getter function for background path.
     * @param type The background type.
     * @return The background path.
     */
    [[nodiscard]] const std::string& background_path(params::BackgroundType type) const noexcept;

    /**
     * @brief Getter function for background tree name.
     * @param type The background type.
     * @return The background tree name.
     */
    [[nodiscard]] const std::string& background_tree_name(params::BackgroundType type) const noexcept;

    /**
     * @brief Getter function for background branch name.
     * @param type The background type.
     * @return The background branch name.
     */
    [[nodiscard]] const std::string& background_branch_name(params::BackgroundType type) const noexcept;

    /**
     * @brief Function for setting up input paths.
     * @param description The program options description.
     */
    void setup_inputPaths(boost::program_options::options_description& description);

   private:
    std::string m_DetectorSection;
    std::string m_DataPath;
    std::string m_OffOffDataPath;
    std::string m_EvisName;
    std::string m_ReactorPath;
    std::string m_ReactorTree;
    std::string m_ReactorVisualEnergyName;
    std::string m_ReactorTrueEnergyName;
    std::string m_ReactorDistanceName;
    std::string m_ReactorGDMLName;
    std::string m_ReactorCovPath;
    std::string m_DNCPath;
    std::string m_AccidentalOffPath;

    std::vector<std::string> m_CovarianceMatrixPath;
    std::vector<std::string> m_CovarianceMatrixName;

    std::vector<std::string> m_BackgroundPath;
    std::vector<std::string> m_BackgroundTree;
    std::vector<std::string> m_BackgroundBranch;
  };

}  // namespace io