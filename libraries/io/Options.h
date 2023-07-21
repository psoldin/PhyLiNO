#pragma once

namespace io {

  class Options {
   public:
    /**
     * Constructor
     * @param argc Command line argc
     * @param argv Command line argv
     */
    Options(int argc, char** argv);

    /** Default constructor */
    Options()
      : Options(1, nullptr) {}

    /** Default destructor */
    ~Options() = default;
  };

}  // namespace io