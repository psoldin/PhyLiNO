#include "DCInputOptions.h"

// boost includes
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace io::dc {

  DCInputOptions::DCInputOptions()
    : InputOptionBase("Double Chooz Options") {
    setup_options();
  }

  void DCInputOptions::setup_options() {
    namespace po = boost::program_options;
    m_Options.add_options()
    ("dc.useData", po::bool_switch(&m_UseData), "Use Double Chooz Measurement Data")
    ("dc.addSystematicalErrors,y", po::bool_switch(&m_UseSystematicErrors), "Use Systematic Errors for toy-Spectra creation")
    ("dc.addStatisticalErrors,t", po::bool_switch(&m_UseStatisticalErrors), "Use Statistics Errors for toy-Spectra creation")
    ("dc.fakeBump", po::bool_switch(&m_FakeBump), "Add fake bump to fake data")
    ("dc.llhScan", po::bool_switch(&m_LikelihoodScan), "Perform a likelihood scan")
    ("dc.useSterile", po::bool_switch(&m_UseSterile), "Use Sterile Neutrino Parameters")
    ("dc.reactorSplit,r", po::bool_switch(&m_ReactorSplit), "Use reactor split")
    ("dc.config,c", po::value<std::string>(&m_ConfigFile)->default_value("dc_config.json")->required(), "Set Double Chooz Config file path")
    ;
  }

  void DCInputOptions::read(const boost::program_options::variables_map& vm) {
    namespace pt = boost::property_tree;

    if (!boost::filesystem::exists(m_ConfigFile)) {
      throw std::invalid_argument("Error: Config File " + m_ConfigFile + " not found");
    }

    std::cout << "Reading Double Chooz Config File: " << m_ConfigFile << '\n';
    pt::read_json(m_ConfigFile, m_ConfigTree);

    /*
    m_InputParameter = std::make_unique<InputParameter>(tree.get_child("Parameters"));

    for (const auto& [section_name, paths] : tree.get_child("Inputs")) {
      std::cout << "--- Reading Input Data for '" << section_name << "' ---\n";

      if (section_name == "DoubleChooz") {
        try {
          for (const auto& [path_name, path] : paths) {
            // This is the name of the section, so ND, FDI and FDII in the first part.
            // The second part are the individual paths in the same format. It is strictly required that they are in the same format.
            m_InputPaths.emplace(get_detectorType_from_String(path_name), dc::InputPaths(path_name, path));
          }
        } catch (std::exception& e) {
          std::cout << e.what() << '\n';
        }
      }
    }
   */

  }

} // namespace io::dc
