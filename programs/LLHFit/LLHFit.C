// STL includes
#include <iostream>

#include "Options.h"

int main(int argc, char** argv) {

  io::Options options(argc, argv);

  std::cout << options.inputOptions().seed() << std::endl;

  std::cout << "Hello World!" << std::endl;
}