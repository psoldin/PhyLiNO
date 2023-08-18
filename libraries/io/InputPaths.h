#pragma once

// STL includes
#include <string>

namespace io {

  class InputPaths {
   public:
    InputPaths(std::string&& section)
      : m_DetectorSection(std::move(section)) {}

    ~InputPaths() = default;

    [[nodiscard]] const std::string& detector_section() const noexcept { return m_DetectorSection; }

    [[nodiscard]] const std::string& data_path() const noexcept { return m_DataPath; }

    [[nodiscard]] const std::string& data_offoff_path() const noexcept { return m_DataOffOffPath; }

   private:
    std::string m_DetectorSection;
    std::string m_DataPath;
    std::string m_DataOffOffPath;
  };

}  // namespace io