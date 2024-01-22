// STL includes
#include <iostream>

#include "Options.h"
#include "Fitter.h"

int main(int argc, char** argv) {

  io::Options options(argc, argv);

  ana::Fitter fit(options);

  std::cout << options.inputOptions().seed() << std::endl;

  std::cout << "Hello World!" << std::endl;
}