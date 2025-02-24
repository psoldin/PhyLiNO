#include "DCInputOptions.h"
#include "DCDetectorPaths.h"

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
    m_Options.add_options()("dc.useData", po::bool_switch(&m_UseData), "Use Double Chooz Measurement Data")
    ("dc.addSystematicalErrors,y", po::bool_switch(&m_UseSystematicErrors), "Use Systematic Errors for toy-Spectra creation")
    ("dc.addStatisticalErrors,t", po::bool_switch(&m_UseStatisticalErrors), "Use Statistics Errors for toy-Spectra creation")
    ("dc.fakeBump", po::bool_switch(&m_FakeBump), "Add fake bump to fake data")
    ("dc.llhScan", po::bool_switch(&m_LikelihoodScan), "Perform a likelihood scan")
    ("dc.useSterile", po::bool_switch(&m_UseSterile), "Use Sterile Neutrino Parameters")
    ("dc.reactorSplit,r", po::bool_switch(&m_ReactorSplit), "Use reactor split");
  }

  void DCInputOptions::read(const boost::program_options::variables_map& vm, const boost::property_tree::ptree& config) {
    std::cout << "Reading Double Chooz Input Paths\n";

    auto get_detectorType_from_String = [](const std::string& detector) -> params::dc::DetectorType {
      if (detector == "ND") {
        return params::dc::DetectorType::ND;
      }
      if (detector == "FDI") {
        return params::dc::DetectorType::FDI;
      }
      if (detector == "FDII") {
        return params::dc::DetectorType::FDII;
      }
      throw std::invalid_argument("Invalid detector type");
    };

    for (const auto& [section_name, paths] : config.get_child("DoubleChooz")) {
      try {
        m_InputPaths.emplace(get_detectorType_from_String(section_name), DCDetectorPaths(section_name, paths));
      } catch (const boost::property_tree::ptree_bad_path& e) {
        std::cout << e.what() << std::endl;
      }
    }

    for (const auto& [section, inputPaths] : m_InputPaths) {
      std::cout << inputPaths << '\n';
    }
  }

}  // namespace io::dc
