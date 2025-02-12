#pragma once

#include "DataBase.h"

namespace io::dc {

  class DCOptions {
  public:
    explicit DCOptions(InputOptions& inputOptions) : m_DataBase(inputOptions) {}

    ~DCOptions() = default;

    [[nodiscard]] const DataBase& dataBase() const noexcept { return m_DataBase; }

  private:
    DataBase m_DataBase;
  };

} // namespace io::dc