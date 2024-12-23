#pragma once

#include <Eigen/Core>

namespace ana::dc {

  template <int nBins=44>
  using return_t = Eigen::Array<double, nBins, 1>;

}