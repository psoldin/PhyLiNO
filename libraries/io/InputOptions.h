#pragma once

// includes
#include "InputParameter.h"
#include "InputPaths.h"

namespace io {

  // TODO Documentation

  class InputOptions {
   public:
    /** Constructor that gets the command line arguments
     * @param argc
     * @param argv
     */
    InputOptions(int argc, char** argv);

    /** Default destructor */
    ~InputOptions() = default;

    /** Global random seed for the number generation */
    [[nodiscard]] int seed() const noexcept { return m_Seed; }

    /** Whether to run in silent mode */
    [[nodiscard]] bool silent() const noexcept { return m_Silent; }

   private:
    int m_Seed;
    bool m_Silent;
    bool m_UseData;
    bool mUseSystematicalErrors;
    bool mUseStatisticalErrors;
    bool mFakeBump;
    bool mLikelihoodScan;

    std::string m_ConfigFile;

    std::unique_ptr<InputParameter> m_InputParameter;
    std::vector<InputPaths>         m_InputPaths;
  };
}  // namespace io
