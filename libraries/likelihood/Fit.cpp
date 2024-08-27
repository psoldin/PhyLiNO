#include "Fit.h"

namespace ana {

  Fit::Fit(std::shared_ptr<io::Options> options)
    : m_Options(std::move(options)) {}

}  // namespace ana