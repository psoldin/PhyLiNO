#pragma once

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
    };
  }  // namespace io

