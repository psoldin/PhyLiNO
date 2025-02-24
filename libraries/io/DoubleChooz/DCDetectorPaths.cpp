#include "DCDetectorPaths.h"
#include "../Parameter.h"

// STL includes
#include <iomanip>
#include <iostream>
#include <string>

// boost includes
#include <boost/program_options.hpp>

namespace io::dc {

  DCDetectorPaths::DCDetectorPaths(std::string section, const boost::property_tree::ptree& tree)
    : m_DetectorSection(std::move(section)) {
    // Define enum for less writing
    using enum params::dc::BackgroundType;
    try {
      // Read the paths from the property tree for backgrounds
      for (const auto background : {accidental, lithium, fastN}) {
        std::string name = params::dc::get_background_name(background);
        m_BackgroundPath.push_back(tree.get<std::string>(name));
        m_BackgroundTree.push_back(tree.get<std::string>(name + "_tree"));
        m_BackgroundBranch.push_back(tree.get<std::string>(name + "_branch"));
        m_CovarianceMatrixPath.push_back(tree.get<std::string>(name + "_cov"));
        m_CovarianceMatrixName.push_back(tree.get<std::string>(name + "_cov_name"));
      }

      // Double Neutron Capture data
      m_DNCPath    = tree.get<std::string>("dnc");
      m_DNCName_Hy = tree.get<std::string>("dnc_Hy");
      m_DNCName_Gd = tree.get<std::string>("dnc_Gd");

      // Reactor data
      m_ReactorPath          = tree.get<std::string>("reactor");
      m_ReactorTree          = tree.get<std::string>("reactor_tree");
      m_ReactorCovPath       = tree.get<std::string>("reactor_cov");
      m_ReactorCovName       = tree.get<std::string>("reactor_cov_name");
      m_Reactor_energy       = tree.get<std::string>("reactor_branch_energy");
      m_Reactor_visualEnergy = tree.get<std::string>("reactor_branch_visualEnergy");
      m_Reactor_distance     = tree.get<std::string>("reactor_branch_distance");
      m_Reactor_GDML         = tree.get<std::string>("reactor_branch_GDML");

      // Data files
      m_DataPath       = tree.get<std::string>("data");
      m_OffOffDataPath = tree.get<std::string>("data_off");
    } catch (const boost::property_tree::ptree_bad_path& e) {
      std::cout << e.what() << '\n';
    }
  }

  std::ostream& operator<<(std::ostream& os, const DCDetectorPaths& paths) {
    os << "Paths for '" << paths.detector_section() << "':\n";

    using enum params::dc::BackgroundType;

    auto bkgType_to_string = [](params::dc::BackgroundType type) -> std::string {
      switch (type) {
        case accidental:
          return "Accidental";
        case lithium:
          return "Lithium";
        case fastN:
          return "Fast Neutron";
        case dnc:
          return "Delayed Neutron Capture";
        default:
          return "Unknown";
      }
    };

    for (auto bkg : {accidental, lithium, fastN}) {
      const std::string name = bkgType_to_string(bkg);
      os << std::setw(40) << (name + " Path: ") << paths.background_path(bkg) << "\n";
      os << std::setw(40) << (name + " Tree: ") << paths.background_tree_name(bkg) << "\n";
      os << std::setw(40) << (name + " Branch: ") << paths.background_branch_name(bkg) << "\n";
      os << std::setw(40) << (name + " Cov: ") << paths.covarianceMatrix_path(bkg) << "\n";
      os << std::setw(40) << (name + " Cov Name: ") << paths.covarianceMatrix_name(bkg) << "\n";
    }

    os << std::setw(40) << "Double Neutron Capture Path: " << paths.dnc_path() << "\n";
    os << std::setw(40) << "Double Neutron Capture Name Hy: " << paths.dnc_name_Hy() << "\n";
    os << std::setw(40) << "Double Neutron Capture Name Gd: " << paths.dnc_name_Gd() << "\n";

    os << std::setw(40) << "Reactor Path: " << paths.reactor_neutrino_data_path() << "\n";
    os << std::setw(40) << "Reactor Tree Name: " << paths.reactor_neutrino_tree_name() << "\n";
    os << std::setw(40) << "Reactor Branch Energy: " << paths.reactor_branch_trueEnergy() << "\n";
    os << std::setw(40) << "Reactor Branch Visual Energy: " << paths.reactor_branch_visualEnergy() << "\n";
    os << std::setw(40) << "Reactor Branch Distance: " << paths.reactor_branch_distance() << "\n";
    os << std::setw(40) << "DataPath: " << paths.data_path() << "\n";
    os << std::setw(40) << "OffOffPath: " << paths.data_offoff_path() << "\n";

    return os;
  }
}  // namespace io::dc
