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

  InputPaths::InputPaths(std::string section, const boost::property_tree::ptree& tree)
    : m_DetectorSection(std::move(section)) {
    // Define enum for less writing
    using enum params::BackgroundType;
    try {
      for (auto background : {accidental, lithium, fastN}) {
        std::string name = m_DetectorSection + "." + params::get_background_name(background);
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
      m_DataPath       = tree.get<std::string>(m_DetectorSection + ".data");
      m_OffOffDataPath = tree.get<std::string>(m_DetectorSection + ".data_off");

      // Reactor data
      m_ReactorPath    = tree.get<std::string>(m_DetectorSection + ".reactor");
      m_ReactorTree    = tree.get<std::string>(m_DetectorSection + ".reactor_tree");
      m_ReactorCovPath = tree.get<std::string>(m_DetectorSection + ".reactor_cov");
      m_ReactorCovName = tree.get<std::string>(m_DetectorSection + ".reactor_cov_name");

      // Double Neutron Capture data
      m_DNCPath = tree.get<std::string>(m_DetectorSection + ".dnc");
      m_DNCName = tree.get<std::string>(m_DetectorSection + ".dnc_name");
    } catch (std::exception& e) {
      std::cout << e.what() << '\n';
    }
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
