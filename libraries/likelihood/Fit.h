#pragma once

#include "Likelihood.h"
#include "Options.h"

// STL includes
#include <chrono>
#include <memory>
#include <vector>

// ROOT includes
#include <Math/Factory.h>
#include <Math/Functor.h>
#include <Math/Minimizer.h>

#include "DoubleChooz/DCLikelihood.h"

namespace ana {
  // TODO: Implementation & Documentation missing

  class Fit {
   public:
    explicit Fit(std::shared_ptr<io::Options> options);

    ~Fit() = default;

    [[nodiscard]] std::shared_ptr<dc::Likelihood> doublechooz_likelihood() const;

    bool minimize();

    const dc::DCLikelihood& likelihood() const;

    [[nodiscard]] double time_duration() const;

    [[nodiscard]] bool converged() const;

    [[nodiscard]] const std::shared_ptr<io::Options>& options() const;

    auto get_minimizer() const { return m_Minimizer; }

    bool use_double_chooz() const { return true; }  // TODO This should be initialized in the Constructor

   private:
    std::shared_ptr<io::Options> m_Options;

    std::chrono::duration<double, std::ratio<1>> m_FitDuration;

    bool m_Converged;
    bool m_FitPerformed;

    std::shared_ptr<ROOT::Math::Minimizer> m_Minimizer;

    std::shared_ptr<ROOT::Math::Functor> m_Functor;

    std::shared_ptr<dc::Likelihood> m_DCLikelihood;

    void setup_minimizer();
  };

}  // namespace ana
