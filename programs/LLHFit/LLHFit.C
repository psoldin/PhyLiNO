// STL includes
#include <iostream>
#include <ranges>

// includes
#include "Fit.h"
#include "Options.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "DoubleChooz/DCLikelihood.h"

#include "write_results.h"

#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TROOT.h>

#include <numeric>

int main(int argc, char** argv) {
  ROOT::EnableThreadSafety();

  // try {
  auto options = std::make_shared<io::Options>(argc, argv);

  ana::Fit fit(options);

  fit.minimize();
  result::write_results(fit, "Output");

  std::cout << "####\t" << fit.get_minimizer()->X()[0] << '\n';
  // } catch (std::exception& e) {
  //   std::cout << e.what() << '\n';
  return EXIT_FAILURE;
  // }
  return EXIT_SUCCESS;
}
