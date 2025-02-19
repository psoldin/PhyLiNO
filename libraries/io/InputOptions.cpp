#include "InputOptions.h"

// STL includes
#include <chrono>
#include <iostream>

// boost includes
#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem.hpp>

namespace io {

  InputOptions::InputOptions(int argc, char** argv)
    : m_Seed(std::chrono::system_clock::now().time_since_epoch().count())
    , m_Silent(false)
    , m_MultiThreadingCores(-1)
  {
    std::string inputFile;
    try {
      namespace po = boost::program_options;
      namespace pt = boost::property_tree;

      const long current_time = std::chrono::system_clock::now().time_since_epoch().count();

      po::options_description generic_options("Options");

      generic_options.add_options()
      ("help,h", "Print help message")
      ("config,c", po::value<std::string>(&m_ConfigFile)->default_value("config.json")->required(), "Set Config File")
      ("seed", po::value<long>(&m_Seed)->default_value(current_time), "Set seed for simulation")
      ("silent", po::bool_switch(&m_Silent), "Run fit in silence mode")
      ("multiThreading,m", po::value<int>(&m_MultiThreadingCores)->default_value(1), "Use multiple threads for fitting")
      ;

      po::variables_map vm;
      store(po::parse_command_line(argc, argv, generic_options), vm);

      if (vm.count("help")) {
        std::stringstream ss;
        ss << "Basic Command Line Parameter App\n"
           << generic_options << std::endl;

        throw std::logic_error(ss.str());
      }

      notify(vm);

      if (!boost::filesystem::exists(m_ConfigFile)) {
        throw std::invalid_argument("Error: Config File " + m_ConfigFile + " not found");
      }

      pt::ptree tree;
      read_json(m_ConfigFile, tree);

      m_InputParameter = std::make_unique<InputParameter>(tree.get_child("Parameters"));

      auto get_detectorType_from_String = [](const std::string& name) {
        if (name == "ND")
          return params::dc::DetectorType::ND;
        if (name == "FDI")
          return params::dc::DetectorType::FDI;
        if (name == "FDII")
          return params::dc::DetectorType::FDII;

        throw std::invalid_argument("Error: DetectorType " + name + " not known");
      };

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

    } catch (std::exception& e) {
      std::cout << e.what() << '\n';
    }
  }
}  // namespace io::dc