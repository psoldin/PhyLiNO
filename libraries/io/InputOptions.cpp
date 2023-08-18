#pragma once

#include "InputOptions.h"

// STL includes
#include <iostream>

// boost includes
#include <boost/program_options.hpp>

namespace io {

  InputOptions::InputOptions(int argc, char** argv) {
    namespace po = boost::program_options;

    try {
      po::options_description generic_options("Generic Options");
      generic_options.add_options()
        ("silent", "Run fit in silence mode without output");
    } catch (std::exception& e) {
      std::cout << e.what() << '\n';
    }
  }

}  // namespace io