#include "InputOptions.h"

// STL includes
#include <chrono>
#include <iostream>

// boost includes
#include <boost/program_options.hpp>

namespace io {

  InputOptions::InputOptions(int argc, char** argv) {
    namespace po = boost::program_options;
    using namespace std::chrono;

    try {
      const int seed = static_cast<int>(system_clock::now().time_since_epoch().count());
      po::options_description generic_options("Generic Options");
      generic_options.add_options()
      ("silent", po::bool_switch(&m_Silent), "Run fit in silence mode")
      ("seed", po::value<int>(&m_Seed)->default_value(seed), "Set seed for simulation");
    } catch (std::exception& e) {
      std::cout << e.what() << '\n';
    }
  }

}  // namespace io