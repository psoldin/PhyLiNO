#pragma once

namespace io {

  /**
   * Container class for the information contained in the input ROOT files. This contains the information for a
   * single neutrino event.
   * The visual energy, true neutrino energy, distance and a coding for the interaction volume (GDML) is introduced.
   */
  struct TreeEntry {
  public:
    /**
     * Constructor for the information contained in this container class.
     * @param e_vis visual neutrino energy (i.e. the IBD prompt energy) measured in the detector
     * @param e_true true neutrino energy from MC simulation
     * @param distance travelled distance of the neutrino from reactor to detector
     * @param gdml encoding for the volume the neutrino interacted in. This is later used for MC corrections
     */
    TreeEntry(double e_vis, double e_true, double distance, int gdml)
      : Evis(e_vis)
      , Etrue(e_true)
      , Distance(distance)
      , GDML(gdml) {}

    /** Default destructor */
    ~TreeEntry() = default;

    /** Default copy constructor */
    TreeEntry(const TreeEntry&) = default;

    /** Default move constructor */
    TreeEntry(TreeEntry&&) = default;

    /** Default copy assignment operator */
    TreeEntry& operator=(const TreeEntry&) = default;

    /** Default move assignment operator */
    TreeEntry& operator=(TreeEntry&&) = default;

    double Evis;      // visual/measured neutrino energy
    double Etrue;     // true neutrino energy
    double Distance;  // travelled neutrino distance
    int    GDML;      // encoding for the interaction volume
  };

}