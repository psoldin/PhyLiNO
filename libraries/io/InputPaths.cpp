#include "InputPaths.h"
#include "Parameter.h"

// STL includes
#include <iomanip>
#include <iostream>
#include <string>

// boost includes
#include <boost/program_options.hpp>

void print_argument(std::ostream& os, std::string_view name, std::string_view argument) {
  os << '\t' << std::left << std::setw(30) << name << argument << std::endl;
}

namespace io {

  void InputPaths::setup_inputPaths(boost::program_options::options_description& description) {
    namespace po = boost::program_options;

    int ac_idx = static_cast<int>(params::BackgroundType::accidental);
    int li_idx = static_cast<int>(params::BackgroundType::lithium);
    int fn_idx = static_cast<int>(params::BackgroundType::fastN);

    description.add_options()
    ((m_DetectorSection + ".Data_Path").c_str(), po::value<std::string>(&m_DataPath), "Data Path")
    ((m_DetectorSection + ".Off_Off_Path").c_str(), po::value<std::string>(&m_OffOffDataPath), "Off-Off Data Path")
    ((m_DetectorSection + ".Vis_E_Branch_Name").c_str(), po::value<std::string>(&m_ReactorVisualEnergyName), "Name of the visual energy branch in ROOT tree")
    ((m_DetectorSection + ".True_E_Branch_Name").c_str(), po::value<std::string>(&m_ReactorTrueEnergyName), "Name of the true neutrino energy branch in ROOT tree")
    ((m_DetectorSection + ".Distance_Branch_Name").c_str(), po::value<std::string>(&m_ReactorDistanceName), "Name of neutrino distance branch in ROOT tree")
    ((m_DetectorSection + ".GDML_Branch_Name").c_str(), po::value<std::string>(&m_ReactorGDMLName), "Name of GDML volume encoding branch in ROOT tree")
    ((m_DetectorSection + ".Reactor_Data_Path").c_str(), po::value<std::string>(&m_ReactorPath), "Reactor Data input Path")
    ((m_DetectorSection + ".Reactor_Tree_Name").c_str(), po::value<std::string>(&m_ReactorTree), "Tree name for the reactor data in ROOT file")
    ((m_DetectorSection + ".Reactor_Cov_Path").c_str(), po::value<std::string>(&m_ReactorCovPath), "Reactor convariance matrix input Path")
    ((m_DetectorSection + ".Accidental_Data_Path").c_str(), po::value<std::string>(&m_BackgroundPath[ac_idx]), "Path for Accidental Bkg input file")
    ((m_DetectorSection + ".Accidental_Off_Data_Path").c_str(), po::value<std::string>(&m_AccidentalOffPath), "Path for Accidental Bkg Off-Off input file")
    ((m_DetectorSection + ".Accidental_Cov_Path").c_str(), po::value<std::string>(&m_CovarianceMatrixPath[ac_idx]), "Path for Accidental covariance matrix input file")
    ((m_DetectorSection + ".Accidental_Tree_Name").c_str(), po::value<std::string>(&m_BackgroundTree[ac_idx]), "Accidental background tree name in ROOT file")
    ((m_DetectorSection + ".Accidental_Branch_Name").c_str(), po::value<std::string>(&m_BackgroundBranch[ac_idx]), "Accidental background branch name in ROOT file")
    ((m_DetectorSection + ".Lithium_Data_Path").c_str(), po::value<std::string>(&m_BackgroundPath[li_idx]), "Path for Lithium Bkg input file")
    ((m_DetectorSection + ".Lithium_Cov_Path").c_str(), po::value<std::string>(&m_CovarianceMatrixPath[li_idx]), "Path for Lithium Covariance Matrix input file")
    ((m_DetectorSection + ".Lithium_Tree_Name").c_str(), po::value<std::string>(&m_BackgroundTree[li_idx]), "Accidental lithium tree name in ROOT file")
    ((m_DetectorSection + ".Lithium_Branch_Name").c_str(), po::value<std::string>(&m_BackgroundBranch[li_idx]), "Accidental lithium tree branch in ROOT file")
    ((m_DetectorSection + ".DNC_Data_Path").c_str(), po::value<std::string>(&m_DNCPath), "Path for Double Neutron Capture Data input file")
    ((m_DetectorSection + ".FNSM_Data_Path").c_str(), po::value<std::string>(&m_BackgroundPath[fn_idx]), "Path for FNSM Data input file")
    ((m_DetectorSection + ".FNSM_Cov_Path").c_str(), po::value<std::string>(&m_CovarianceMatrixPath[fn_idx]), "Path for FNSM Covariance Matrix input file")
    ((m_DetectorSection + ".FNSM_Tree_Name").c_str(), po::value<std::string>(&m_BackgroundTree[fn_idx]), "Accidental lithium tree in ROOT file")
    ((m_DetectorSection + ".FNSM_Branch_Name").c_str(), po::value<std::string>(&m_BackgroundBranch[fn_idx]), "Accidental lithium tree branch in ROOT file");
  }

  const std::string& InputPaths::covarianceMatrix_name(params::BackgroundType type) const {
    if (type == params::BackgroundType::dnc)
      throw std::invalid_argument("No Covariance Matrix is used for the DNC background");

    return m_CovarianceMatrixName[static_cast<int>(type)];
  }

  const std::string& InputPaths::background_path(params::BackgroundType type) const noexcept {
    return m_BackgroundPath[static_cast<int>(type)];
  }

  const std::string& InputPaths::background_tree_name(params::BackgroundType type) const noexcept {
    return m_BackgroundTree[static_cast<int>(type)];
  }

  const std::string& InputPaths::background_branch_name(params::BackgroundType type) const noexcept {
    return m_BackgroundBranch[static_cast<int>(type)];
  }

}  // namespace io