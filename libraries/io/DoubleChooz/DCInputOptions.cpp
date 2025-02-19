#include "DCInputOptions.h"

namespace io::dc {

  DCInputOptions::DCInputOptions()
    : InputOptionBase("Double Chooz Options") {
    setup_options();
  }

  void DCInputOptions::setup_options() {
    namespace po = boost::program_options;
    m_Options.add_options()
    ("dc.useData", po::bool_switch(&m_UseData), "Use Double Chooz Measurement Data")
    ("dc.addSystematicalErrors,y", po::bool_switch(&m_UseSystematicErrors), "Use Systematic Errors for toy-Spectra creation")
    ("dc.addStatisticalErrors,t", po::bool_switch(&m_UseStatisticalErrors), "Use Statistics Errors for toy-Spectra creation")
    ("dc.fakeBump", po::bool_switch(&m_FakeBump), "Add fake bump to fake data")
    ("dc.llhScan", po::bool_switch(&m_LikelihoodScan), "Perform a likelihood scan")
    ("dc.useSterile", po::bool_switch(&m_UseSterile), "Use Sterile Neutrino Parameters")
    ("dc.reactorSplit,r", po::bool_switch(&m_ReactorSplit), "Use reactor split")
    ;
  }

  void DCInputOptions::read(const boost::program_options::variables_map& vm) {

  }

} // namespace io::dc
