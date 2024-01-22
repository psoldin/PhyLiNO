#pragma once

// includes
#include "Parameter.h"

// STL includes
#include <fstream>
#include <iostream>
#include <string>

// boost includes
#include <boost/property_tree/ptree.hpp>

namespace io {

  /**
   * @class InputPaths
   * @brief Class representing input paths for a detector.
   * This class stores the paths to various input files and data for a detector.
   * It is used to initialize the paths by reading them from a boost::property_tree.
   */
  class InputPaths {
   public:
    /**
     * @brief Constructor for InputPaths class.
     * @param section The section name.
     * @param tree The boost::property_tree object.
     */
    InputPaths(std::string section, const boost::property_tree::ptree& tree);

    /**
     * @brief Destructor for InputPaths class.
     */
    ~InputPaths() = default;

    /**
     * @brief Getter function for detector section name.
     * @return The detector section name.
     */
    [[nodiscard]] const std::string& detector_section() const noexcept;

    /**
     * @brief Getter function for data path.
     * @return The data path.
     */
    [[nodiscard]] const std::string& data_path() const noexcept {
      return m_DataPath;
    }

    /**
     * @brief Getter function for off-off data path.
     * @return The off-off data path.
     */
    [[nodiscard]] const std::string& data_offoff_path() const noexcept;

    /**
     * @brief Getter function for reactor neutrino data path.
     * @return The reactor neutrino data path.
     */
    [[nodiscard]] const std::string& reactor_neutrino_data_path() const noexcept;

    /**
     * @brief Getter function for neutrino visual energy branch name.
     * @return The neutrino visual energy branch name.
     */
    [[nodiscard]] const std::string& neutrino_visual_energy_branch() const noexcept;

    /**
     * @brief Getter function for neutrino true energy branch name.
     * @return The neutrino true energy branch name.
     */
    [[nodiscard]] const std::string& neutrino_true_energy_branch() const noexcept;

    /**
     * @brief Getter function for neutrino distance branch name.
     * @return The neutrino distance branch name.
     */
    [[nodiscard]] const std::string& neutrino_distance_branch() const noexcept;

    /**
     * @brief Getter function for neutrino GDML branch name.
     * @return The neutrino GDML branch name.
     */
    [[nodiscard]] const std::string& neutrino_GDML_branch() const noexcept;

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

    [[nodiscard]] const std::string& reactor_branch_visualEnergy() const noexcept {
      return m_ReactorVisualEnergyName;
    }

    [[nodiscard]] const std::string& reactor_branch_trueEnergy() const noexcept {
      return m_ReactorTrueEnergyName;
    }

    [[nodiscard]] const std::string& reactor_branch_distance() const noexcept {
      return m_ReactorDistanceName;
    }

    [[nodiscard]] const std::string& reactor_branch_GDML() const noexcept {
      return m_ReactorGDMLName;
    }

    /**
     * @brief Getter function for reactor neutrino tree name.
     * @return The reactor neutrino tree name.
     */
    [[nodiscard]] const std::string& reactor_neutrino_tree_name() const noexcept {
      return m_ReactorTree;
    }

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
    std::string m_ReactorCovName;
    std::string m_DNCPath;
    std::string m_DNCName;
    std::string m_AccidentalOffPath;
    std::string m_Reactor_energy;
    std::string m_Reactor_visualEnergy;
    std::string m_Reactor_distance;
    std::string m_Reactor_GDML;

    std::vector<std::string> m_CovarianceMatrixPath;
    std::vector<std::string> m_CovarianceMatrixName;

    std::vector<std::string> m_BackgroundPath;
    std::vector<std::string> m_BackgroundTree;
    std::vector<std::string> m_BackgroundBranch;
  };

}  // namespace io
