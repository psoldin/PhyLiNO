// STL includes
#include <iostream>
#include <ranges>

// includes
#include "Fit.h"
#include "Options.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "DoubleChooz/DCLikelihood.h"

#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>

#include <numeric>

int main(int argc, char** argv) {

  auto options = std::make_shared<io::Options>(argc, argv);

  ana::Fit fit(options);

  fit.minimize();

  std::cout << "####\t" << fit.get_minimizer()->X()[0] << '\n';
}