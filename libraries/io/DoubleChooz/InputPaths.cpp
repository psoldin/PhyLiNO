#include "InputPaths.h"
#include "../Parameter.h"

// STL includes
#include <iomanip>
#include <iostream>
#include <string>

// boost includes
#include <boost/program_options.hpp>

void print_argument(std::ostream& os, std::string_view name, std::string_view argument) {
  os << '\t' << std::left << std::setw(30) << name << argument << std::endl;
}

namespace io::dc {

  InputPaths::InputPaths(std::string section, const boost::property_tree::ptree& tree)
    : m_DetectorSection(std::move(section)) {
    // Define enum for less writing

    // Loop through all entries in tree and print key-value pairs
    // for (const auto& entry : tree) {
    //   std::cout << "Key: " << entry.first << ", Value: " << entry.second.get_value<std::string>() << std::endl;
    // }

    using enum params::dc::BackgroundType;
    try {
      for (const auto background : {accidental, lithium, fastN}) {
        std::string name = params::dc::get_background_name(background);
        m_BackgroundPath.push_back(tree.get<std::string>(name));
        m_BackgroundTree.push_back(tree.get<std::string>(name + "_tree"));
        m_BackgroundBranch.push_back(tree.get<std::string>(name + "_branch"));
        m_CovarianceMatrixPath.push_back(tree.get<std::string>(name + "_cov"));
        m_CovarianceMatrixName.push_back(tree.get<std::string>(name + "_cov_name"));

        if (background == accidental) {
          m_BackgroundPath.push_back(tree.get<std::string>(name + "_off"));
        }
      }

      // Data files
      m_DataPath       = tree.get<std::string>("data");
      m_OffOffDataPath = tree.get<std::string>("data_off");

      // Reactor data
      m_ReactorPath          = tree.get<std::string>("reactor");
      m_ReactorTree          = tree.get<std::string>("reactor_tree");
      m_ReactorCovPath       = tree.get<std::string>("reactor_cov");
      m_ReactorCovName       = tree.get<std::string>("reactor_cov_name");
      m_Reactor_energy       = tree.get<std::string>("reactor_branch_energy");
      m_Reactor_visualEnergy = tree.get<std::string>("reactor_branch_visualEnergy");
      m_Reactor_distance     = tree.get<std::string>("reactor_branch_distance");
      m_Reactor_GDML         = tree.get<std::string>("reactor_branch_GDML");

      // Double Neutron Capture data
      m_DNCPath = tree.get<std::string>("dnc");
      m_DNCName = tree.get<std::string>("dnc_name");
    } catch (std::exception& e) {
      std::cout << e.what() << '\n';
    }
  }
}  // namespace io::dc
