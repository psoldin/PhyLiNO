#include "Fitter.h"

namespace ana {

  Fitter::Fitter(std::shared_ptr<io::Options> options)
    : m_Options(std::move(options)) {}

}  // namespace ana