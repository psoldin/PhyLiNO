#include "Options.h"

namespace io {

  Options::Options(int argc, char** argv)
    : m_InputOptions(argc, argv)
    , m_DataBase(m_InputOptions)
    , m_StartingParameter(m_InputOptions) {

  }
}  // namespace io