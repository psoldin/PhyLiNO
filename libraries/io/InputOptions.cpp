#include "InputOptions.h"

// STL includes
#include <chrono>
#include <iostream>

// boost includes
#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace io {

  /*
  InputOptions::InputOptions(int argc, char** argv) {
    namespace po = boost::program_options;
    using namespace std::chrono;

    try {
      const int current_time = std::chrono::system_clock::now().time_since_epoch().count();
      const int seed = static_cast<int>(system_clock::now().time_since_epoch().count());
      po::options_description generic_options("Generic Options");
      generic_options.add_options()
      ("silent", po::bool_switch(&m_Silent), "Run fit in silence mode")
      ("seed", po::value<int>(&m_Seed)->default_value(current_time), "Set seed for simulation")
      ("help,h", "Print help message")
      ("config,c", po::value<std::string>(&m_ConfigFile)->default_value("config.json"), "Set config file");
    } catch (std::exception& e) {
      std::cout << e.what() << '\n';
    }
  }
  */


  InputOptions::InputOptions(int argc, char** argv) {
    std::string inputFile;
    try {
      namespace po = boost::program_options;
      namespace pt = boost::property_tree;

      const int current_time = std::chrono::system_clock::now().time_since_epoch().count();

      po::options_description generic_options("Options");
      

      generic_options.add_options()("help,h", "Print help message")
      ("config,c", po::value<std::string>(&m_ConfigFile)->default_value("config.json"), "Set Config File")
      ("seed", po::value<int>(&m_Seed)->default_value(current_time), "Set seed for simulation")
      ("silent", po::bool_switch(&m_Silent), "Run fit in silence mode")
      ("useData", po::bool_switch(&m_UseData), "Use measurement Data")
      ("addSystematicalErrors,y", po::bool_switch(&m_UseSystematicalErrors), "Use Systematic Errors for toy-Spectra creation")
      ("addStatisticalErrors,t", po::bool_switch(&m_UseStatisticalErrors), "Use Statistics Errors for toy-Spectra creation")
      ("fakeBump,f", po::bool_switch(&m_FakeBump), "Add fake bump to fake data")
      ("llhScan", po::bool_switch(&m_LikelihoodScan), "Perform a likelihood scan")
      ;
      /*
      ("seed", po::value<int>(&mSeed)->default_value(current_time), "Set seed for simulation")
      ("tolerance", po::value<double>(&mTolerance)->default_value(0.05), "Set Fit tolerance")
      ("silent", po::bool_switch(&mSilent), "Run fit in silence mode")
      ("gpu", po::bool_switch(&mUseGPU), "Use GPU for fitting")
      ("useSterile", po::bool_switch(&mUseSterile), "Use Sterile Parameters")
      ("useData", po::bool_switch(&mUseData), "Use measurement Data")
      ("multiThreading,m", po::bool_switch(&mMultiThreading), "Use multiple threads for fitting")
      ("addSystematicalErrors,y", po::bool_switch(&mUseSystematicalErrors), "Use Systematic Errors for toy-Spectra")
      ("addStatisticalErrors,t", po::bool_switch(&mUseStatisticalErrors), "Use Statistics Errors for toy-Spectra")
      ("splitReactors,r", po::bool_switch(&mSplitReactors), "Split reactor data")
      ("smoothBackground,s", po::bool_switch(&mSmoothBackgrounds), "Smooth backgrounds")
      ("fakeBump,f", po::bool_switch(&mFakeBump), "Add fake bump to fake data")
      ("t13CV", po::value<double>(&mT14CV)->default_value(0.0), "Set central value for t14")
      ("dm241CV", po::value<double>(&mDM241CV)->default_value(0.0), "Set central value for dm^{2}_{41}")
      ("dataChallenge", po::value<std::string>(&mDataChallengePath), "Perform the data Challenge from Thiagos data files")
      ("fitVerification", po::bool_switch(&mFitVerification), "Fit verification, so t13_{reconstructed} == t13_{truth} test")
      ("noOffOff", po::bool_switch(&mNoOffOff), "Use Off Off Data for the fit")
      ("statTest", po::bool_switch(&mStatTest), "Perform statistic tests: N-1, stat+1")
      ("llhScan", po::bool_switch(&mLikelihoodScan), "Perform a likelihood scan")
      ("startingParameterTest", po::bool_switch(&mStartingParameterTest), "Test wether fit results depends on starting parameter for t13")
      ("MCModelUncertainty", po::bool_switch(&mMCModelScan), "Scan Output Spectra to estimate the model uncertainity");
      */

      po::variables_map vm;
      po::store(po::parse_command_line(argc, argv, generic_options), vm);

      if (vm.count("help")) {
        std::stringstream ss;
        ss << "Basic Command Line Parameter App\n"
           << generic_options << std::endl;

        throw std::logic_error(ss.str());
      }

      pt::ptree tree;
      pt::read_json(m_ConfigFile, tree);

      m_InputParameter = std::make_unique<InputParameter>(tree.get_child("parameters"));

      for (const auto& section : tree.get_child("paths")) {
        // This is the name of the section, so ND, FDI and FDII in the first part.
        // The second part are the individual paths in the same format. It is stricly required that they are in the same format.
        m_InputPaths.emplace_back(section.first, section.second);
      }

    } catch (std::exception& e) {
      std::cout << e.what() << '\n';
    }
  }
}  // namespace io