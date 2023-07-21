#pragma once

namespace io {

  class Options {
    public:
      Options(int argc, char** argv);

      /** Default constructor */
      Options()
        : Options(1, nullptr) { }

      /** Default destructor */
      ~Options() = default;
  };

} // namespace io