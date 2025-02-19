#include "DataBase.h"

// includes
#include "../TreeEntry.h"

// STL includes
#include <string>

// boost includes
#include <boost/timer/progress_display.hpp>

// ROOT includes
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

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
  std::vector<TreeEntry> read_reactor_root_file(const std::string& filePath, const InputPaths& paths) {
    // Open the root file
    TFile* file = TFile::Open(filePath.c_str());

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
    std::sort(treeEntries.begin(),
              treeEntries.end(),
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

  DataBase::DataBase(const io::InputOptions& inputOptions)
    : m_InputOptions(inputOptions) {
    using enum params::dc::DetectorType;

    for (auto detector : {ND, FDI, FDII}) {
      const auto& path = m_InputOptions.input_paths(detector);

      if (inputOptions.dc_input_options().use_data()) {
        // Read the measurement data from the ROOT file
        const std::string& dataPath   = path.data_path();
        // const std::string& treeName   = path.data_tree_name();
        // const std::string& branchName = path.data_branch_name();
      }
    }
  }

}  // namespace io::dc