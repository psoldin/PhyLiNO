#include "DataBase.h"

// includes
#include "../TreeEntry.h"

// STL includes
#include <random>
#include <ranges>
#include <string>

// boost includes
#include <boost/timer/progress_display.hpp>

// ROOT includes
#include <TFile.h>
#include <TMatrixD.h>
#include <TMatrixDSym.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TVector.h>

namespace io::dc {

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
      } while (value < 1.0 && value > 10.0);
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
    std::exponential_distribution dist(50.0);
    std::vector<double>           samples(num_samples);
    for (std::size_t i = 0; i < num_samples; ++i) {
      samples[i] = dist(gen) + 1.0;
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

    return reactor_tree_entries;
  }

  DataBase::DataBase(const io::InputOptions& inputOptions)
    : m_InputOptions(inputOptions) {
    using enum params::dc::DetectorType;

    std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());

    for (auto detector : {ND, FDI, FDII}) {
      const auto& paths = m_InputOptions.double_chooz().input_paths(detector);

      // std::vector<TreeEntry> reactor_tree_entries = read_reactor_root_file(paths);

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
          p1          = 355.0;
          p2          = 470.0;
          name        = "FDI";
          break;
        case FDII:
          ratio       = 0.55;
          num_samples = 10'000'000;
          p1          = 997.0;
          p2          = 1115.0;
          name        = "FDII";
          break;
        default:;  // Do nothing
      }

      std::cout << "Generating " << std::setw(10) << num_samples << " samples for reactor data set for " << name << '\n';
      auto reactor_tree_entries = generate_reactor_entries(gen, num_samples, ratio, p1, p2);

      m_ReactorData[detector] = std::make_shared<ReactorData>(reactor_tree_entries, detector);
    }

    using enum params::dc::BackgroundType;
    std::cout << "Generating " << std::setw(10) << 40'000 << " samples for Accidental Background\n";
    m_BackgroundData[accidental] = generate_accidental_background(gen, 40'000);

    std::cout << "Generating " << std::setw(10) << 650'000 << " samples for Lithium Background\n";
    m_BackgroundData[lithium] = generate_lithium_background(gen, 650'000);

    std::cout << "Generating " << std::setw(10) << 2'000'000 << " samples for fastN Background\n";
    m_BackgroundData[fastN] = generate_fastN_background(gen, 2'000'000);
  }

}  // namespace io::dc