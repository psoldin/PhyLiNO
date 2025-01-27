#include "InputOptions.h"

// STL includes
#include <chrono>
#include <iostream>

// boost includes
#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace io::dc {

  InputOptions::InputOptions(int argc, char** argv) {
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
      ("useData", po::bool_switch(&m_UseData), "Use measurement Data")
      ("addSystematicalErrors,y", po::bool_switch(&m_UseSystematicalErrors), "Use Systematic Errors for toy-Spectra creation")
      ("addStatisticalErrors,t", po::bool_switch(&m_UseStatisticalErrors), "Use Statistics Errors for toy-Spectra creation")
      ("fakeBump,f", po::bool_switch(&m_FakeBump), "Add fake bump to fake data")
      ("llhScan", po::bool_switch(&m_LikelihoodScan), "Perform a likelihood scan")
      ("useSterile", po::bool_switch(&m_UseSterile), "Use Sterile Neutrino Parameters")
      ;
      /*
      ("seed", po::value<int>(&mSeed)->default_value(current_time), "Set seed for simulation")
      ("tolerance", po::value<double>(&mTolerance)->default_value(0.05), "Set Fit tolerance")
      ("silent", po::bool_switch(&mSilent), "Run fit in silence mode")
      ("gpu", po::bool_switch(&mUseGPU), "Use GPU for fitting")
      ("useSterile", po::bool_switch(&mUseSterile), "Use Sterile Parameters")
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

      po::notify(vm);

      if (!boost::filesystem::exists(m_ConfigFile)) {
        throw std::invalid_argument("Error: Config File " + m_ConfigFile + " not found");
      }

      pt::ptree tree;
      pt::read_json(m_ConfigFile, tree);

      m_InputParameter = std::make_unique<InputParameter>(tree.get_child("parameters"));

      auto get_detectorType_from_String = [](const std::string& name) {
        if (name == "ND")
          return params::dc::DetectorType::ND;
        else if (name == "FDI")
          return params::dc::DetectorType::FDI;
        else if (name == "FDII")
          return params::dc::DetectorType::FDII;
        else
          throw std::invalid_argument("Error: DetectorType " + name + " not known");
      };

      for (const auto& section : tree.get_child("paths")) {
        // This is the name of the section, so ND, FDI and FDII in the first part.
        // The second part are the individual paths in the same format. It is stricly required that they are in the same format.
        m_InputPaths.emplace(get_detectorType_from_String(section.first), InputPaths(section.first, section.second));
      }

    } catch (std::exception& e) {
      std::cout << e.what() << '\n';
    }
  }
}  // namespace io::dc