#include "Fit.h"

namespace ana {

  Fit::Fit(std::shared_ptr<io::Options> options)
    : m_Options(std::move(options))
    , m_FitDuration(0)
    , m_Converged(false)
    , m_FitPerformed(false) {
    // Lock the mutex to ensure that the minimizer is not created in parallel due to ROOT limitations
    static std::mutex mutex;
    std::unique_lock  lock{mutex};

    // Initialize Likelihood
    // TODO Make this dynamic
    m_DCLikelihood = std::make_shared<dc::DCLikelihood>(m_Options);

    // Create the minimizer
    m_Minimizer = std::shared_ptr<ROOT::Math::Minimizer>(ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad"));

    m_Functor = std::make_shared<ROOT::Math::Functor>(m_DCLikelihood.get(),
                                                      &dc::Likelihood::calculate_likelihood,
                                                      params::number_of_parameters());

    m_Minimizer->SetFunction(*m_Functor);

    m_Minimizer->SetTolerance(m_Options->inputOptions().tolerance());

    setup_minimizer();
  }

  void Fit::setup_minimizer() {
    using namespace params;
    using namespace params::dc;

    const bool silent = m_Options->inputOptions().silent();

    const auto& input_parameters = m_Options->inputOptions().input_parameters();

    const auto& names      = input_parameters.names();
    const auto& fixed      = input_parameters.fixed();
    const auto& parameters = input_parameters.parameters();

    for (std::size_t i = 0; i < parameters.size(); ++i) {
      if (!silent) {
        std::cout << "Set up parameter " << std::setw(5) << i << ": " << std::setw(18) << names[i]
                  << " with value " << std::setw(10) << parameters[i].value()
                  << " and uncertainty " << parameters[i].uncertainty() << '\n';
      }

      m_Minimizer->SetVariable(i, names[i], parameters[i].value(), parameters[i].uncertainty());
    }

    const bool use_sterile = m_Options->inputOptions().double_chooz().use_sterile();

    if (!silent) {
      std::cout << "-----\n";
    }

    for (std::size_t i = 0; i < parameters.size(); ++i) {
      if (use_sterile && (i == DeltaM41 || i == SinSqT14))
        continue;

      if (fixed[i]) {
        if (!silent) {
          std::cout << "Fixing parameter " << std::setw(5) << i << " " << names[i] << '\n';
        }
        m_Minimizer->FixVariable(i);
      }
    }
  }

  std::shared_ptr<dc::Likelihood> Fit::doublechooz_likelihood() const {
    return m_DCLikelihood;
  }

  bool Fit::minimize() {
    using namespace std::chrono;

    m_Minimizer->SetPrintLevel(m_Options->inputOptions().silent() ? 0 : 2);

    const auto begin = high_resolution_clock::now();
    m_Converged      = m_Minimizer->Minimize();
    const auto end   = high_resolution_clock::now();

    m_FitDuration = end - begin;

    std::stringstream ss;
    ss << "Fit finished: " << std::boolalpha << m_Converged << '\n';
    ss << "It took: " << m_FitDuration.count() << " seconds\n";
    ss << "Likelihood: " << m_Minimizer->MinValue() << '\n';
    ss << "EDM: " << m_Minimizer->Edm() << '\n';

    std::cout << ss.rdbuf() << std::endl;

    m_FitPerformed = true;

    return m_Converged;
  }

  const dc::DCLikelihood& Fit::likelihood() const {
    if (!m_DCLikelihood)
      throw std::logic_error("DCLikelihood not initialized");

    return *std::static_pointer_cast<dc::DCLikelihood>(m_DCLikelihood);
  }

  double Fit::time_duration() const {
    if (!m_FitPerformed)
      std::cout << "Fit not performed yet\n";

    return m_FitDuration.count();
  }

  bool Fit::converged() const {
    if (!m_FitPerformed)
      std::cout << "Fit not performed yet\n";

    return m_Converged;
  }

  const io::Options& Fit::options() const {
    if (!m_FitPerformed)
      std::cout << "Fit not performed yet\n";

    return *m_Options;
  }

}  // namespace ana