#pragma once

// includes
#include "../Parameter.h"

// STL includes
#include <fstream>
#include <iostream>
#include <string>

// boost includes
#include <boost/property_tree/ptree.hpp>

namespace io::dc {

  /**
   * @class DCDetectorPaths
   * @brief Class representing input paths for a detector.
   * This class stores the paths to various input files and data for a detector.
   * It is used to initialize the paths by reading them from a boost::property_tree.
   */
  class DCDetectorPaths {
   public:
    /**
     * @brief Constructor for InputPaths class.
     * @param section The section name.
     * @param input_options The input options.
     */
    DCDetectorPaths(std::string section, const boost::property_tree::ptree& tree);

    /**
     * @brief Destructor for InputPaths class.
     */
    ~DCDetectorPaths() = default;

    /**
     * @brief Getter function for detector section name.
     * @return The detector section name.
     */
    [[nodiscard]] const std::string& detector_section() const noexcept {
      return m_DetectorSection;
    }

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
    [[nodiscard]] const std::string& data_offoff_path() const noexcept {
      return m_OffOffDataPath;
    }

    /**
     * @brief Getter function for reactor neutrino data path.
     * @return The reactor neutrino data path.
     */
    [[nodiscard]] const std::string& reactor_neutrino_data_path() const noexcept {
      return m_ReactorPath;
    }

    /**
     * @brief Getter function for neutrino visual energy branch name.
     * @return The neutrino visual energy branch name.
     */
    [[nodiscard]] const std::string& neutrino_visual_energy_branch() const noexcept {
      return m_Reactor_visualEnergy;
    }

    /**
     * @brief Getter function for neutrino true energy branch name.
     * @return The neutrino true energy branch name.
     */
    [[nodiscard]] const std::string& neutrino_true_energy_branch() const noexcept {
      return m_Reactor_energy;
    }

    /**
     * @brief Getter function for neutrino distance branch name.
     * @return The neutrino distance branch name.
     */
    [[nodiscard]] const std::string& neutrino_distance_branch() const noexcept {
      return m_Reactor_distance;
    }

    /**
     * @brief Getter function for neutrino GDML branch name.
     * @return The neutrino GDML branch name.
     */
    [[nodiscard]] const std::string& neutrino_GDML_branch() const noexcept {
      return m_Reactor_GDML;
    }

    /**
     * @brief Getter function for covariance matrix path.
     * @param type The background type.
     * @return The covariance matrix path.
     */
    [[nodiscard]] const std::string& covarianceMatrix_path(params::dc::BackgroundType type) const {
      return m_CovarianceMatrixPath[static_cast<int>(type)];
    }

    /**
     * @brief Getter function for covariance matrix name.
     * @param type The background type.
     * @return The covariance matrix name.
     */
    [[nodiscard]] const std::string& covarianceMatrix_name(params::dc::BackgroundType type) const {
      return m_CovarianceMatrixName[static_cast<int>(type)];
    }

    /**
     * @brief Getter function for background path.
     * @param type The background type.
     * @return The background path.
     */
    [[nodiscard]] const std::string& background_path(params::dc::BackgroundType type) const noexcept {
      return m_BackgroundPath[static_cast<int>(type)];
    }

    /**
     * @brief Getter function for background tree name.
     * @param type The background type.
     * @return The background tree name.
     */
    [[nodiscard]] const std::string& background_tree_name(params::dc::BackgroundType type) const noexcept {
      return m_BackgroundTree[static_cast<int>(type)];
    }

    /**
     * @brief Getter function for background branch name.
     * @param type The background type.
     * @return The background branch name.
     */
    [[nodiscard]] const std::string& background_branch_name(params::dc::BackgroundType type) const noexcept {
      return m_BackgroundBranch[static_cast<int>(type)];
    }

    [[nodiscard]] const std::string& reactor_branch_visualEnergy() const noexcept {
      return m_Reactor_visualEnergy;
    }

    [[nodiscard]] const std::string& reactor_branch_trueEnergy() const noexcept {
      return m_Reactor_energy;
    }

    [[nodiscard]] const std::string& reactor_branch_distance() const noexcept {
      return m_Reactor_distance;
    }

    [[nodiscard]] const std::string& reactor_branch_GDML() const noexcept {
      return m_Reactor_GDML;
    }

    /**
     * @brief Getter function for reactor neutrino tree name.
     * @return The reactor neutrino tree name.
     */
    [[nodiscard]] const std::string& reactor_neutrino_tree_name() const noexcept {
      return m_ReactorTree;
    }

    /**
     * @brief Getter function for reactor covariance matrix path.
     * @return The reactor covariance matrix path.
     */
    [[nodiscard]] const std::string& reactor_covariance_matrix_path() const noexcept {
      return m_ReactorCovPath;
    }

    /**
     * @brief Getter function for reactor covariance matrix name in ROOT file.
     * @return The reactor covariance matrix name.
     */
    [[nodiscard]] const std::string& reactor_covariance_matrix_name() const noexcept {
      return m_ReactorCovName;
    }

    /**
     * @brief Getter function for accidental off data path.
     * @return The accidental off data path.
     */
    [[nodiscard]] const std::string& accidental_off_data_path() const noexcept {
      return m_AccidentalOffPath;
    }

    /**
     * @brief Getter function for the Double Neutron Capture data file path
     * @return The Double Neutron Capture data file path
     */
    [[nodiscard]] const std::string& dnc_path() const noexcept {
      return m_DNCPath;
    }

    /** Getter function for the Double Neutron Capture histogram name for Hydrogen
     * @return The Double Neutron Capture histogram name for Hydrogen
     */
    [[nodiscard]] const std::string& dnc_name_Hy() const noexcept {
      return m_DNCName_Hy;
    }

    /** Getter function for the Double Neutron Capture histogram name for Gadolinium
     * @return The Double Neutron Capture histogram name for Gadolinium
     */
    [[nodiscard]] const std::string& dnc_name_Gd() const noexcept {
      return m_DNCName_Gd;
    }

    /** Stream operator for DCDetectorPaths
     * @param os The output stream
     * @param paths The DCDetectorPaths object
     * @return The output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const DCDetectorPaths& paths);

   private:
    std::string m_DetectorSection;
    std::string m_DataPath;
    std::string m_OffOffDataPath;
    std::string m_ReactorPath;
    std::string m_ReactorTree;
    std::string m_ReactorCovPath;
    std::string m_ReactorCovName;
    std::string m_DNCPath;
    std::string m_DNCName_Hy;
    std::string m_DNCName_Gd;
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

}  // namespace io::dc
