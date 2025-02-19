#pragma once

#include <boost/program_options.hpp>

namespace io {

  class InputOptionBase {
   public:
    InputOptionBase(std::string name)
    : m_Options(std::move(name)) { }

    virtual ~InputOptionBase() = default;

    auto& options() noexcept { return m_Options; }

    virtual void read(const boost::program_options::variables_map& vm) = 0;

   protected:
    boost::program_options::options_description m_Options;
  };

}