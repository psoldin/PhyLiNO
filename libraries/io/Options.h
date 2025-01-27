#pragma once

#include "DoubleChooz/DataBase.h"
#include "InputOptions.h"
#include "StartingParameter.h"
#include "DoubleChooz/DCOptions.h"

namespace io {

  /**
   * Main class for the options that are handled in the Likelihood fit.
   * Here the command line arguments as well as the measurement data are accessible.
   */
  class Options {
   public:
    /**
     * Constructor
     * @param argc Command line argc
     * @param argv Command line argv
     */
    Options(int argc, char** argv)
      : m_InputOptions(argc, argv)
      , m_DCOptions(m_InputOptions) {}

    /** Default constructor */
    Options()
      : Options(1, nullptr) {}

    /** Default destructor */
    ~Options() = default;

    [[nodiscard]] const dc::DCOptions& dc_options() const noexcept { return m_DCOptions; }

    [[nodiscard]] const InputOptions& inputOptions() const noexcept { return m_InputOptions; }

   private:
    InputOptions m_InputOptions;
    dc::DCOptions m_DCOptions;
  };

}  // namespace io