#pragma once

// STL includes
#include <cstddef>

namespace ana {

  class ParameterWrapper {
   public:
    explicit ParameterWrapper(const double* parameter)
      : m_RawParameter(parameter) { }

    virtual ~ParameterWrapper() = default;

    [[nodiscard]] const double* raw_parameter() const noexcept { return m_RawParameter; }

    [[nodiscard]] double operator[](std::size_t index) const noexcept { return m_RawParameter[index]; }

   protected:
    const double* m_RawParameter;       // Pointer to the parameter array
  };

}