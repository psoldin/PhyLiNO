#include "DataBase.h"

// includes
#include "TreeEntry.h"

// STL includes
#include <string>

// boost includes
#include <boost/timer/progress_display.hpp>

// ROOT includes
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

namespace io {

  // Reads a branch from a root file with a given file path
  // Returns a pointer to the TTree object containing the branch
  std::vector<TreeEntry> read_reactor_root_file(const std::string& filePath, const std::string& treeName) {
    // Open the root file
    TFile* file = TFile::Open(filePath.c_str());

    // Throw an exception if the file could not be opened
    if (!file)
      throw std::invalid_argument("Error: could not open file " + filePath);

    // Get the TTree object containing the branch
    TTree* tree = dynamic_cast<TTree*>(file->Get("tree_name"));

    // Throw an exception if the TTree object could not be found
    if (!tree)
      throw std::invalid_argument("Error: could not find TTree object in file " + treeName);

    // Get the number of entries in the TTree object
    const auto NEntries = tree->GetEntries();

    // Create a TTreeReader object to read the TTree object
    TTreeReader reader(treeName.c_str(), file);

    // Create TTreeReaderValue objects to read the values of the branches
    TTreeReaderValue<double> visibleEnergy(reader, "myPromptEvisID");
    TTreeReaderValue<double> trueEnergy(reader, "myNeutrinoEnergy_Th");
    TTreeReaderValue<double> distance(reader, "myNeutrinoDistance_Th");
    TTreeReaderValue<int>    volumeGDML(reader, "volumeGDML");

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
        
      }
  }

}