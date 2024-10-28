#pragma once

// STL includes
#include <array>
#include <vector>

namespace io {

  class Constants {
   public:
    Constants() = default;

    /**
     * @brief Destructor for the Constants class.
     *
     * This destructor is set to default.
     */
    ~Constants() = default;

    /**
     * @brief Energy bin x-axis values used in the Double Chooz experiment.
     *
     * This static constexpr std::array<double, 45> contains the energy bin x-axis values used in the Double Chooz experiment.
     * The values are in MeV and range from 1.0 to 50.0 MeV.
     */
    static constexpr std::array<double, 45> EnergyBinXaxis = {1.0, 1.25, 1.5, 1.75, 2., 2.25, 2.5,
                                                              2.75, 3., 3.25, 3.5, 3.75, 4., 4.25,
                                                              4.5, 4.75, 5., 5.25, 5.5, 5.75, 6., 6.25,
                                                              6.5, 6.75, 7., 7.25, 7.5, 7.75, 8.0, 8.5,
                                                              9., 9.5, 10., 11., 12., 14., 16., 18., 20.,
                                                              25., 30., 35., 40., 45., 50.};

    static constexpr double DistanceB1ND = 468.761;  // meter (DocDB# 6286 slide 7)

    static constexpr double DistanceB2ND = 355.388;  // meter (DocDB# 6286 slide 7)

    static constexpr double DistanceB1FD = 1114.656;  // meter (DocDB# 6286 slide 7)

    static constexpr double DistanceB2FD = 997.839;  // meter (DocDB# 6286 slide 7)

    static constexpr int number_of_energy_bins = 38;  // Number of energy bins used in the Double Chooz experiment
  };

}  // namespace io