#pragma once

// includes
#include "DataBase.h"
#include "StartingParameter.h"

namespace io::dc {

  class DCOptions {
  public:
    explicit DCOptions(const InputOptions& inputOptions)
      : m_DataBase(inputOptions)
      , m_StartingParameter(inputOptions) { }

    ~DCOptions() = default;

    [[nodiscard]] const DataBase& dataBase() const noexcept {
      return m_DataBase;
    }

    [[nodiscard]] const StartingParameter& starting_parameters() const noexcept {
      return m_StartingParameter;
    }

  private:
    DataBase m_DataBase;
    StartingParameter m_StartingParameter;
  };

} // namespace io::dc