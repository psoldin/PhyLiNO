#include "InputOptions.h"

// STL includes
#include <chrono>
#include <iostream>

// boost includes
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace io {

  InputOptions::InputOptions(int argc, char** argv)
    : m_Seed(std::chrono::system_clock::now().time_since_epoch().count())
    , m_Silent(false)
    , m_MultiThreadingCores(-1) {
    std::string inputFile;
    try {
      namespace po = boost::program_options;
      namespace pt = boost::property_tree;

      const long current_time = std::chrono::system_clock::now().time_since_epoch().count();

      po::options_description generic_options("Options");

      generic_options.add_options()("help,h", "Print help message")
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

      using option_ptr_t = std::shared_ptr<InputOptionBase>;
      namespace pt       = boost::property_tree;

      if (!boost::filesystem::exists(m_ConfigFile)) {
        throw std::invalid_argument("Error: Config File " + m_ConfigFile + " not found");
      }

      std::cout << "Reading Config File: " << m_ConfigFile << '\n';
      pt::read_json(m_ConfigFile, m_ConfigTree);

      m_InputParameter = std::make_shared<InputParameter>(m_ConfigTree.get_child("Parameter"));

      // Add the experiment specific options here
      std::vector<option_ptr_t> options = {m_DCInputOptions};

      for (auto option : options) {
        if (option == nullptr)
          continue;

        option->read(vm, m_ConfigTree);
      }

    } catch (std::exception& e) {
      std::cout << e.what() << '\n';
    }
  }
}  // namespace io