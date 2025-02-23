#pragma once

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>

namespace io {

  class InputOptionBase {
   public:
    InputOptionBase(std::string name)
    : m_Options(std::move(name)) { }

    virtual ~InputOptionBase() = default;

    auto& options() noexcept { return m_Options; }

    virtual void read(const boost::program_options::variables_map& vm, const boost::property_tree::ptree& config) = 0;

   protected:
    boost::program_options::options_description m_Options;
  };

}