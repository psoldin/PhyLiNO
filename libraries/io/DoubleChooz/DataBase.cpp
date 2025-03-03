#include "DataBase.h"

// includes
#include <DoubleChooz/Constants.h>
#include "../TreeEntry.h"

// STL includes
#include <random>
#include <ranges>
#include <string>

// boost includes
#include <boost/timer/progress_display.hpp>

// ROOT includes
#include <TFile.h>
#include <TH1D.h>
#include <TMatrixD.h>
#include <TMatrixDSym.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TVector.h>

namespace io::dc {

  std::shared_ptr<Eigen::MatrixXd> get_bkg_cov_matrix(std::span<const double> backgroundSpectrum, int nBins = Constants::number_of_energy_bins) {
    auto histogram = std::make_unique<TH1D>("h",
                                            "",
                                            nBins,
                                            Constants::EnergyBinXaxis.data());

    for (auto E : backgroundSpectrum) {
      histogram->Fill(E);
    }

    auto cv = std::move(histogram);

    int    Nbins = cv->GetNbinsX();
    double Ntot  = cv->Integral(0, Nbins);

    // For ease of handling, make N_i vector
    TVectorD N(Nbins);
    for (int b = 0; b < Nbins; b++) {
      N[b] = cv->GetBinContent(b + 1);
    }

    // Construct total, normalization, and shape matrices
    auto Mtot = std::make_unique<TMatrixD>(Nbins, Nbins);
    Mtot->Zero();

    for (int i = 0; i < Nbins; i++) {
      (*Mtot)(i, i) = std::pow(cv->GetBinError(i + 1), 2);
    }

    const double Msum = Mtot->Sum();

    auto Mnorm  = std::make_unique<TMatrixD>(Nbins, Nbins);
    auto Mshape = std::make_unique<TMatrixD>(Nbins, Nbins);
    auto Mmixed = std::make_unique<TMatrixD>(Nbins, Nbins);

    for (int i = 0; i < Nbins; i++) {
      for (int j = 0; j < Nbins; j++) {
        (*Mnorm)(i, j) = N[i] * N[j] / pow(Ntot, 2) * Msum;

        (*Mshape)(i, j) = (*Mtot)(i, j) - N[j] / Ntot * (*Mtot)(i, i) - N[i] / Ntot * (*Mtot)(j, j) + N[i] * N[j] / pow(Ntot, 2) * Msum;

        (*Mmixed)(i, j) = N[j] / Ntot * (*Mtot)(i, i) + N[i] / Ntot * (*Mtot)(j, j) - 2 * N[i] * N[j] / pow(Ntot, 2) * Msum;
      }
    }

    // Shape+mixed actually is the "shape matrix" for final fit
    auto Mffit = std::make_unique<TMatrixD>(*Mshape);
    (*Mffit) += (*Mmixed);

    auto Mffit_frac = std::make_unique<TMatrixD>(*Mffit);

    // Fractionalize the ffit matrix
    for (int i = 0; i < Nbins; i++) {
      for (int j = 0; j < Nbins; j++) {
        if (N[i] == 0 || N[j] == 0)
          (*Mffit_frac)(i, j) = 0;
        else
          (*Mffit_frac)(i, j) *= 1. / (N[i] * N[j]);
      }
    }

    auto fracCovMatrix = std::make_shared<Eigen::MatrixXd>(nBins, nBins);

    for (unsigned int i = 0; i < nBins; ++i) {
      for (unsigned int j = 0; j < nBins; ++j) {
        (*fracCovMatrix)(i, j) = (*Mffit_frac)(i, j);
      }
    }

    return fracCovMatrix;
  }

  /**
   * @brief Represents a dynamic array of double values.
   *
   * This class provides a container to store and manipulate a collection of double values.
   * It supports various operations such as adding elements, accessing elements by index,
   * sorting, and resizing.
   */
  std::vector<double> read_measurement_data(const std::string& path, const std::string& tree_name, const std::string& branch_name) {
    // Open the ROOT file at the given path
    auto* file = TFile::Open(path.c_str(), "READ");
    // Check if the file was opened successfully
    assert(file != nullptr);

    // Get the TTree object with the given name from the file
    auto* tree = dynamic_cast<TTree*>(file->Get(tree_name.c_str()));
    // Check if the TTree object was found
    assert(tree);

    // Get the number of entries in the TTree object
    const auto NEntries = tree->GetEntries();

    // Create a TTreeReader object to read the TTree object
    TTreeReader reader(tree_name.c_str(), file);

    // Create a TTreeReaderValue object to read the values of the "myPromptEvisID" branch
    TTreeReaderValue<double> value(reader, branch_name.c_str());

    // Create a vector to store the visible energy entries
    std::vector<double> entries;
    entries.reserve(NEntries);

    // Loop over the entries in the TTree object
    unsigned int counter = 0;
    while (reader.Next()) {
      // Add the visible energy value to the vector
      entries.push_back(*value);
    }

    // Sort the vector of visible energy entries in ascending order
    std::sort(std::begin(entries),
              std::end(entries),
              [](const auto& a, const auto& b) { return a < b; });

    // Close the file and delete the TFile object
    file->Close();
    delete file;

    // Return the vector of visible energy entries
    return entries;
  }

  // Reads a branch from a root file with a given file path
  // Returns a pointer to the TTree object containing the branch
  std::vector<TreeEntry> read_reactor_root_file(const DCDetectorPaths& paths) {
    // Open the root file
    const std::string& filePath = paths.reactor_neutrino_data_path();
    TFile*             file     = TFile::Open(filePath.c_str());

    // Throw an exception if the file could not be opened
    if (!file)
      throw std::invalid_argument("Error: could not open file " + filePath);

    // Get the TTree object containing the branch
    const std::string& treeName = paths.reactor_neutrino_tree_name();
    TTree*             tree     = dynamic_cast<TTree*>(file->Get(treeName.c_str()));

    // Throw an exception if the TTree object could not be found
    if (!tree)
      throw std::invalid_argument("Error: could not find TTree object in file " + treeName);

    // Get the number of entries in the TTree object
    const auto NEntries = tree->GetEntries();

    // Create a TTreeReader object to read the TTree object
    TTreeReader reader(treeName.c_str(), file);

    // Create TTreeReaderValue objects to read the values of the branches
    const std::string& true_Energy_branch   = paths.reactor_branch_trueEnergy();
    const std::string& visual_Energy_branch = paths.reactor_branch_visualEnergy();
    const std::string& distance_branch      = paths.reactor_branch_distance();
    const std::string& branch_GDML          = paths.reactor_branch_GDML();

    TTreeReaderValue<double> visibleEnergy(reader, true_Energy_branch.c_str());
    TTreeReaderValue<double> trueEnergy(reader, visual_Energy_branch.c_str());
    TTreeReaderValue<double> distance(reader, distance_branch.c_str());
    TTreeReaderValue<int>    volumeGDML(reader, branch_GDML.c_str());

    // Print the name of the file being read
    if (auto idx = filePath.rfind('/', filePath.length()); idx != std::string::npos) {
      std::cout << "Reading tree '" << treeName << "' in : " << filePath.substr(idx + 1, filePath.length() - 1) << '\n';
    }

    // Create a progress bar to show the progress of reading the TTree object
    boost::timer::progress_display show_progress(NEntries);

    // Create a vector to store the TreeEntry objects
    std::vector<TreeEntry> treeEntries;
    treeEntries.reserve(NEntries);

    // Loop over the entries in the TTree object
    while (reader.Next()) {
      // Create a TreeEntry object and add it to the vector
      treeEntries.emplace_back(*visibleEnergy, *trueEnergy, *distance, *volumeGDML);
      // Update the progress bar
      ++show_progress;
    }

    // Sort the vector of TreeEntry objects by visible energy
    std::ranges::sort(treeEntries,
                      [](const TreeEntry& a, const TreeEntry& b) {
                        return a.Evis < b.Evis;
                      });

    // Close the file and delete the TFile object
    file->Close();
    delete file;

    // Return the vector of TreeEntry objects
    return treeEntries;
  }

  /**
   * Returns a vector of visible energy entries from a ROOT TTree.
   *
   * @param path The path to the ROOT file containing the TTree.
   * @param treeName The name of the TTree.
   * @return A vector of visible energy entries.
   */
  std::vector<double> getBackgroundEntries(const std::string& path, const std::string& treeName) {
    // Open the ROOT file at the given path
    auto* file = TFile::Open(path.c_str(), "READ");
    // Check if the file was opened successfully
    assert(file != nullptr);

    // Get the TTree object with the given name from the file
    auto* tree = dynamic_cast<TTree*>(file->Get(treeName.c_str()));
    // Check if the TTree object was found
    assert(tree);

    // Get the number of entries in the TTree object
    const auto NEntries = tree->GetEntries();

    // Create a TTreeReader object to read the TTree object
    TTreeReader reader(treeName.c_str(), file);

    // Create a TTreeReaderValue object to read the values of the "myPromptEvisID" branch
    TTreeReaderValue<double> visibleEnergy(reader, "myPromptEvisID");

    // Create a vector to store the visible energy entries
    std::vector<double> entries;
    entries.reserve(NEntries);

    // Loop over the entries in the TTree object
    unsigned int counter = 0;
    while (reader.Next()) {
      // Add the visible energy value to the vector
      entries.push_back(*visibleEnergy);
    }

    // Sort the vector of visible energy entries in ascending order
    std::sort(std::begin(entries),
              std::end(entries),
              [](const auto& a, const auto& b) { return a < b; });

    // Close the file and delete the TFile object
    file->Close();
    delete file;

    // Return the vector of visible energy entries
    return entries;
  }

  void DataBase::construct_energy_correlation_matrix() {
    TMatrixD    corrMatrix(7, 7);
    TVectorD    eigenValues(7);
    TMatrixD    eigenVectors(corrMatrix.EigenVectors(eigenValues));
    TMatrixDSym eigenValueMatrix(7);

    for (int i = 0; i < 7; ++i) {
      eigenValueMatrix(i, i) = std::sqrt(eigenValues(i));
    }

    m_EnergyCorrelationMatrix.Mult(eigenVectors, eigenValueMatrix);
  }

  std::vector<double> generate_lithium_background(std::default_random_engine& gen, std::size_t num_samples) {
    std::normal_distribution dist(5.0, 2.0);
    std::vector<double>      sample(num_samples);
    for (std::size_t i = 0; i < num_samples; ++i) {
      double value;
      do {
        value = dist(gen);
      } while (value < 1.0 || value > 10.0);
      sample[i] = value;
    }
    return sample;
  }

  std::vector<double> generate_accidental_background(std::default_random_engine& gen, std::size_t num_samples) {
    std::exponential_distribution dist(1.0);
    std::vector<double>           sample(num_samples);
    for (std::size_t i = 0; i < num_samples; ++i) {
      sample[i] = dist(gen) + 1.0;
    }
    return sample;
  }

  std::vector<double> generate_fastN_background(std::default_random_engine& gen, std::size_t num_samples) {
    std::exponential_distribution dist(1.0 / 50.0);
    std::vector<double>           samples(num_samples);

    double sample_value;
    for (std::size_t i = 0; i < num_samples; ++i) {
      do {
        sample_value = dist(gen) + 1.0;
      } while (sample_value < 1.0 || sample_value > 50.0);

      samples[i] = sample_value;
    }
    return samples;
  }

  std::vector<TreeEntry> generate_reactor_entries(std::default_random_engine& gen, std::size_t num_samples, double ratio, double p1, double p2) {
    std::normal_distribution dist_truth(4.0, 1.5);

    std::vector<double> nuE_truth(num_samples);
    std::vector<double> nuE_vis(num_samples);

    double truth_value, reco_value;
    for (std::size_t i = 0; i < num_samples; ++i) {
      do {
        truth_value = dist_truth(gen);
      } while (truth_value < 0.0);
      nuE_truth[i] = truth_value;

      std::normal_distribution reco_dist(truth_value, 0.5);
      do {
        reco_value = reco_dist(gen);
      } while (reco_value < 0.0);
      nuE_vis[i] = reco_value;
    }

    std::vector<double> distances(num_samples);
    const std::size_t   num_dist1 = static_cast<std::size_t>(ratio * num_samples);

    std::normal_distribution dist_dist1(p1, 1.0);
    for (std::size_t i = 0; i < num_dist1; ++i) {
      distances[i] = dist_dist1(gen);
    }
    std::normal_distribution dist_dist2(p2, 1.0);
    for (std::size_t i = num_dist1; i < num_samples; ++i) {
      distances[i] = dist_dist2(gen);
    }

    std::ranges::shuffle(distances, gen);

    std::vector<TreeEntry> reactor_tree_entries;
    reactor_tree_entries.reserve(num_samples);
    for (std::size_t i = 0; i < num_samples; ++i) {
      reactor_tree_entries.emplace_back(nuE_vis[i], nuE_truth[i], distances[i], 0);
    }

    std::ranges::sort(reactor_tree_entries,
                      [](const auto& a, const auto& b) { return a.Evis < b.Evis; });

    return reactor_tree_entries;
  }

  // Eigen::MatrixXd generate_reactor_covariance_matrix(const std::vector<double>& binnedSpectrum) {
  Eigen::MatrixXd generate_reactor_covariance_matrix(std::span<const double> samples) {
    auto h = std::make_unique<TH1D>("h", "", io::dc::Constants::number_of_energy_bins, io::dc::Constants::EnergyBinXaxis.data());

    for (auto E : samples) {
      h->Fill(E);
    }

    std::vector<double> binnedSpectrum(38, 0.0);

    for (int i = 0; i < 38; ++i) {
      binnedSpectrum[i] = 0.01 * h->GetBinContent(i + 1);
    }

    constexpr int numBins = 38;
    if (binnedSpectrum.size() != numBins) {
      throw std::invalid_argument("Input spectrum must have exactly 38 bins.");
    }

    Eigen::MatrixXd covarianceMatrix = Eigen::MatrixXd::Zero(numBins, numBins);

    // Define uncertainties that match the extracted pattern

    // Variance scaling based on extracted trend
    for (int i = 0; i < numBins; ++i) {
      double varianceScaling = 1.0 + 3.0 * std::exp(2.0 * (static_cast<double>(i) / numBins - 1.0));
      double statError       = (binnedSpectrum[i] > 0) ? varianceScaling * std::sqrt(binnedSpectrum[i]) : 1.0;
      covarianceMatrix(i, i) = statError * statError;

      for (int j = 0; j < numBins; ++j) {
        if (i != j) {
          constexpr double shapeUncertainty = 0.008;
          constexpr double normUncertainty  = 0.015;
          // Normalization correlation
          covarianceMatrix(i, j) += normUncertainty * binnedSpectrum[i] * normUncertainty * binnedSpectrum[j];

          // Shape uncertainty correlation
          double shapeCorrFactor = shapeUncertainty * std::exp(-std::abs(i - j) / 3.0);
          covarianceMatrix(i, j) += shapeCorrFactor * std::sqrt(covarianceMatrix(i, i) * covarianceMatrix(j, j));

          // Localized adjacent bin correlation
          if (std::abs(i - j) == 1) {
            constexpr double neighborCorr = 0.05;
            covarianceMatrix(i, j) += neighborCorr * std::sqrt(covarianceMatrix(i, i) * covarianceMatrix(j, j));
          }
        }
      }
    }

    // Fractionalize the covariance matrix
    using matrix_t                = Eigen::MatrixXd;
    matrix_t fractionalCovariance = matrix_t::Zero(43, 43);
    for (int i = 0; i < numBins; ++i) {
      for (int j = 0; j < numBins; ++j) {
        fractionalCovariance(i, j) = covarianceMatrix(i, j);
        if (binnedSpectrum[i] == 0 || binnedSpectrum[j] == 0) {
          fractionalCovariance(i, j) = 0;
        } else {
          fractionalCovariance(i, j) /= (binnedSpectrum[i] * binnedSpectrum[j]);
        }
      }
    }

    return fractionalCovariance;
  }

  DataBase::DataBase(const io::InputOptions& inputOptions)
    : m_InputOptions(inputOptions) {
    using enum params::dc::DetectorType;

    std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());

    try {
      for (auto detector : {ND, FDI, FDII}) {
        // In the usual case, the input paths are read from the configuration file.
        // Here they are generated on the fly.
        // const auto& paths = m_InputOptions.double_chooz().input_paths(detector);

        std::size_t num_samples;
        double      ratio, p1, p2;
        std::string name;
        switch (detector) {
          case ND:
            ratio       = 0.6;
            num_samples = 6'000'000;
            p1          = 355.0;
            p2          = 470.0;
            name        = "ND";
            break;
          case FDI:
            ratio       = 0.5;
            num_samples = 5'000'000;
            p1          = 997.0;
            p2          = 1115.0;
            name        = "FDI";
            break;
          case FDII:
            ratio       = 0.55;
            num_samples = 10'000'000;
            p1          = 997.0;
            p2          = 1115.0;
            name        = "FDII";
            break;
          default:
            throw std::invalid_argument("Detector type unknown");
        }

        std::cout << "Generating " << std::setw(10) << num_samples << " samples for reactor data set for " << name << '\n';
        auto reactor_tree_entries = generate_reactor_entries(gen, num_samples, ratio, p1, p2);

        m_ReactorData[detector] = std::make_shared<ReactorData>(reactor_tree_entries, detector);

        auto m = generate_reactor_covariance_matrix(m_ReactorData[detector]->evis());

        // Convert matrix to a shared pointer and store it in the map
        m_CovarianceMatrices[{detector, SpectrumType::Reactor}] = std::make_shared<Eigen::MatrixXd>(m);
      }
    } catch (const std::exception& e) {
      std::cout << e.what() << '\n';
      throw;
    }

    using enum params::dc::BackgroundType;
    using enum SpectrumType;

    std::cout << "Generating " << std::setw(10) << 40'000 << " samples for Accidental Background\n";
    {
      m_BackgroundData[Accidental]                            = generate_accidental_background(gen, 40'000);
      auto acc_bkg                                            = get_bkg_cov_matrix(m_BackgroundData[Accidental]);
      m_CovarianceMatrices[std::make_tuple(ND, Accidental)]   = acc_bkg;
      m_CovarianceMatrices[std::make_tuple(FDI, Accidental)]  = acc_bkg;
      m_CovarianceMatrices[std::make_tuple(FDII, Accidental)] = std::move(acc_bkg);
    }

    std::cout << "Generating " << std::setw(10) << 650'000 << " samples for Lithium Background\n";
    {
      m_BackgroundData[Lithium]                            = generate_lithium_background(gen, 650'000);
      auto li_bkg                                          = get_bkg_cov_matrix(m_BackgroundData[Lithium]);
      m_CovarianceMatrices[std::make_tuple(ND, Lithium)]   = li_bkg;
      m_CovarianceMatrices[std::make_tuple(FDI, Lithium)]  = li_bkg;
      m_CovarianceMatrices[std::make_tuple(FDII, Lithium)] = std::move(li_bkg);
    }

    std::cout << "Generating " << std::setw(10) << 2'000'000 << " samples for fastN Background\n";
    {
      m_BackgroundData[FastN]                            = generate_fastN_background(gen, 2'000'000);
      auto fastN_bkg                                     = get_bkg_cov_matrix(m_BackgroundData[FastN], 44);
      m_CovarianceMatrices[std::make_tuple(ND, FastN)]   = fastN_bkg;
      m_CovarianceMatrices[std::make_tuple(FDI, FastN)]  = fastN_bkg;
      m_CovarianceMatrices[std::make_tuple(FDII, FastN)] = std::move(fastN_bkg);
    }

    auto string_to_DetectorType = [](std::string_view name) -> params::dc::DetectorType {
      if (name == "ND") {
        return ND;
      }
      if (name == "FDI") {
        return FDI;
      }
      if (name == "FDII") {
        return FDII;
      }
      throw std::invalid_argument("Invalid detector type");
    };

    std::map<params::dc::DetectorType, std::array<std::pair<double, double>, 3>> energy_central_values;

    try {
      for (const auto& [section, data] : inputOptions.config_tree().get_child("DoubleChooz")) {
        m_OnLifeTime[string_to_DetectorType(section)]  = data.get<double>("on_lifetime");
        m_OffLifeTime[string_to_DetectorType(section)] = data.get<double>("off_lifetime");

        const std::array e = {std::make_pair(data.get<double>("energy_A_CV"), data.get<double>("energy_A_Sig")),
                              std::make_pair(data.get<double>("energy_B_CV"), data.get<double>("energy_B_Sig")),
                              std::make_pair(data.get<double>("energy_C_CV"), data.get<double>("energy_C_Sig"))};

        energy_central_values[string_to_DetectorType(section)] = e;

        const auto mcNorm_Total = std::make_pair(data.get<double>("MCNorm_Total_CV"),
                                                 data.get<double>("MCNorm_Total_Sig"));

        m_MCNormCentralValues[string_to_DetectorType(section)] = mcNorm_Total;
      }
    } catch (const boost::property_tree::ptree_bad_path& e) {
      std::cout << e.what() << '\n';
      throw;
    }

    if (energy_central_values[ND][0] == energy_central_values[FDI][0] && energy_central_values[ND][0] == energy_central_values[FDII][0]) {
      using enum params::General;
      using enum params::dc::Detector;
      // Energy A is the same for all detectors
      m_EnergyCentralValues[EnergyA] = energy_central_values[ND][0];
      // ND parameters
      m_EnergyCentralValues[params::index(ND, EnergyB)] = energy_central_values[ND][1];
      m_EnergyCentralValues[params::index(ND, EnergyC)] = energy_central_values[ND][2];
      // FDI parameters
      m_EnergyCentralValues[params::index(FDI, EnergyB)] = energy_central_values[FDI][1];
      m_EnergyCentralValues[params::index(FDI, EnergyC)] = energy_central_values[FDI][2];
      // FDII parameters
      m_EnergyCentralValues[params::index(FDII, EnergyB)] = energy_central_values[FDII][1];
      m_EnergyCentralValues[params::index(FDII, EnergyC)] = energy_central_values[FDII][2];
    } else {
      throw std::invalid_argument("Energy central values for the EnergyA parameter are not the same for all detectors!");
    }
  }

  std::string get_spectrum_type_string(SpectrumType type) {
    using enum SpectrumType;
    switch (type) {
      case Reactor:
        return "Reactor";
      case Accidental:
        return "Accidental";
      case FastN:
        return "FastN";
      case Lithium:
        return "Lithium";
      case DNC:
        return "DNC";
    }
    throw std::invalid_argument("Spectrum Type not known");
  }

  std::shared_ptr<Eigen::MatrixXd> DataBase::covariance_matrix(params::dc::DetectorType detectorType, SpectrumType spectrumType) const {
    if (!m_CovarianceMatrices.contains({detectorType, spectrumType})) {
      std::stringstream ss;
      ss << "Covariance matrix not found for detector \"" << get_detector_name(detectorType) << "\" and spectrum type \"" << get_spectrum_type_string(spectrumType) << '\"';
      throw std::invalid_argument(ss.str());
    }
    return m_CovarianceMatrices.at({detectorType, spectrumType});
  }

}  // namespace io::dc