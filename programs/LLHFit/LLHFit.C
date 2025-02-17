// STL includes
#include <iostream>
#include <ranges>

// includes
#include "Options.h"
#include "Fit.h"

int main(int argc, char** argv) {
  try {
    auto options = std::make_shared<io::Options>(argc, argv);

    std::cout << std::boolalpha << options->inputOptions().silent() << '\n';

    const auto& parameters = options->inputOptions().input_parameter().parameters();
    const auto& names = options->inputOptions().input_parameter().names();

    for (int i = 0; i < parameters.size(); ++i) {
      std::cout << names[i] << ": " << parameters[i].value() << '\n';
    }

    // ana::Fit fit(options);

    // std::cout << options->inputOptions().seed() << std::endl;

    std::cout << "Hello World!" << std::endl;
  } catch (std::exception& e) {
    std::cout << e.what() << '\n';
  }
}