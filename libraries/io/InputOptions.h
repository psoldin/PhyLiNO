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

    private:
      int m_Seed;
  };

} // namespace io