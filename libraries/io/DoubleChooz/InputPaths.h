#pragma once

#include "../Parameter.h"
#include "DetectorPaths.h"
#include <boost/property_tree/ptree.hpp>

namespace io::dc {

  class InputPaths {
    public:
     explicit InputPaths(const io::InputOptions& inputOptions) {
       const auto& config = inputOptions.double_chooz().config_tree();
       using enum params::dc::DetectorType;

       auto detectorType_as_string = [](params::dc::DetectorType detector) {
         if (detector == ND) return "ND";
         if (detector == FDI) return "FDI";
         if (detector == FDII) return "FDII";
         throw std::runtime_error("Invalid detector type");
       };

       for (auto detector : {ND, FDI, FDII}) {
         m_InputPaths.emplace(detector,
                              DetectorPaths(detectorType_as_string(detector), inputOptions));
       }
     }

     ~InputPaths() = default;

      [[nodiscard]] const DetectorPaths& input_paths(params::dc::DetectorType type) const noexcept {
        return m_InputPaths.at(type);
      }

     private:
      std::unordered_map<params::dc::DetectorType, DetectorPaths> m_InputPaths;
  };

} // namespace io::dc