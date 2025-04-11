#pragma once

#include "Fit.h"

#include <nlohmann/json.hpp>

// STL includes
#include <string_view>

namespace result {

  namespace dc {

    inline nlohmann::json get_json_file(ana::Fit& fit) {
      using namespace nlohmann;

      const auto options = fit.options();
      assert(options != nullptr);

      const auto& inputParameters = options->inputOptions().input_parameters();

      const auto min = fit.get_minimizer();
      assert(min != nullptr);

      const auto& parameter_names = inputParameters.names();
      const auto& parameters      = inputParameters.parameters();
      const auto& constrained     = inputParameters.constrained();

      assert(min->X() != nullptr);

      std::span<const double> X(min->X(), parameters.size());
      std::span<const double> error(min->Errors(), parameters.size());

      json j;

      const auto& dcInfo = options->inputOptions().double_chooz();

      j["valid"]        = min->IsValidError();
      j["nFree"]        = min->NFree();
      j["EDM"]          = min->Edm();
      j["LLH"]          = min->MinValue();
      j["SysErrors"]    = dcInfo.use_systematic_errors();
      j["StatErrors"]   = dcInfo.use_statistical_errors();
      j["FakeBump"]     = dcInfo.fake_bump();
      j["splitReactor"] = dcInfo.reactor_split();
      j["fitTime"]      = std::chrono::system_clock::now().time_since_epoch().count();
      j["fitDuration"]  = fit.time_duration();
      j["converged"]    = fit.converged();

      {
        std::vector<double> binning(io::dc::Constants::EnergyBinXaxis.begin(), io::dc::Constants::EnergyBinXaxis.end());
        binning.insert(binning.end(), {25.0, 30.0, 35.0, 40.0, 45.0, 50.0});
        j["binning"] = binning;
      }

      std::vector<json> parametersJson;

      auto llh = fit.doublechooz_likelihood();

      auto& parameter = llh->parameter();

      parameter.reset_parameter(min->X());

      for (std::size_t i = 0, N = parameters.size(); i < N; ++i) {
        parametersJson.push_back({{"CV", parameters[i].value()},
                                  {"stepWidth", parameters[i].uncertainty()},
                                  {"name", parameter_names[i]},
                                  {"index", i},
                                  {"value", X[i]},
                                  {"fixed", min->IsFixedVariable(i)},
                                  {"constrained", constrained[i]},
                                  {"error", error[i]},
                                  {"true", parameter[i]}});
      }

      j["parameter"] = std::move(parametersJson);

      std::vector<params::dc::DetectorType> detector_types;

      using enum params::dc::DetectorType;

      if (dcInfo.reactor_split()) {
        throw std::invalid_argument("Reactor split is not supported yet.");
      } else {
        detector_types = {ND, FDI, FDII};
      }

      auto dc_llh = fit.doublechooz_likelihood();

      auto& acc_bkg = dc_llh->accidental_background();
      auto& li_bkg  = dc_llh->lithium_background();
      auto& fn_bkg  = dc_llh->fastn_background();
      auto& dnc_bkg = dc_llh->dnc_background();
      auto& signal  = dc_llh->reactor_spectrum();

      std::array<double, 44> signal_prediction{};
      std::ranges::fill(signal_prediction, 0.0);

      std::cout << "Writing default spectra to output file\n";
      // Default Case
      using span_t = std::span<const double>;
      {
        dc_llh->check_and_recalculate(min->X());

        for (const auto detector : detector_types) {
          const span_t acc = acc_bkg.get_spectrum(detector);
          const span_t li  = li_bkg.get_spectrum(detector);
          const span_t fn  = fn_bkg.get_spectrum(detector);
          const span_t dnc = dnc_bkg.get_spectrum(detector);
          const span_t sig = signal.get_spectrum(detector);

          const double mcNorm = dc_llh->calculate_mcNorm(parameter, detector);

          std::transform(sig.begin(),
                         sig.end(),
                         signal_prediction.begin(),
                         [mcNorm](double x) -> double {
                           return x * mcNorm;
                         });

          std::string name = params::dc::get_detector_name(detector);

          json detector_json;

          detector_json["accidental"] = acc;
          detector_json["lithium"]    = li;
          detector_json["fastn"]      = fn;
          detector_json["dnc"]        = dnc;
          detector_json["signal"]     = signal_prediction;

          j[name] = detector_json;
        }
      }
      std::cout << "Writing sginal Null-Hpothesis (signal0) to output file\n";
      std::ranges::fill(signal_prediction, 0.0);

      std::vector<double> input_parameters(params::number_of_parameters(), 0.0);
      std::copy_n(min->X(), params::number_of_parameters(), input_parameters.begin());

      // Null Hypothesis
      using enum params::General;
      input_parameters[SinSqT13] = 0.0;
      {
        dc_llh->check_and_recalculate(input_parameters.data());

        for (const auto detector : detector_types) {
          const span_t sig = signal.get_spectrum(detector);

          const double mcNorm = dc_llh->calculate_mcNorm(parameter, detector);

          std::transform(sig.begin(),
                         sig.end(),
                         signal_prediction.begin(),
                         [mcNorm](double x) -> double {
                           return x * mcNorm;
                         });

          std::string name = params::dc::get_detector_name(detector);

          j[name]["signal0"] = signal_prediction;
        }
      }
      std::cout << "Writing default case without Shape parameter (signal-shape) to output file\n";
      std::ranges::fill(signal_prediction, 0.0);
      std::copy_n(min->X(), params::number_of_parameters(), input_parameters.begin());
      for (const auto detector : {ND, FDI, FDII}) {
        using enum params::dc::Detector;
        for (int i = NuShape01; i <= NuShape43; ++i) {
          input_parameters[params::index(detector, i)] = 0.0;
        }
      }
      {
        dc_llh->check_and_recalculate(input_parameters.data());

        for (const auto detector : detector_types) {
          const span_t sig = signal.get_spectrum(detector);

          const double mcNorm = dc_llh->calculate_mcNorm(parameter, detector);

          std::transform(sig.begin(),
                         sig.end(),
                         signal_prediction.begin(),
                         [mcNorm](double x) -> double {
                           return x * mcNorm;
                         });

          std::string name = params::dc::get_detector_name(detector);

          j[name]["signal-shape"] = signal_prediction;
        }
      }
      std::cout << "Writing Null-Hpothesis case without Shape parameter (signal0-shape) to output file\n";
      std::ranges::fill(signal_prediction, 0.0);
      std::copy_n(min->X(), params::number_of_parameters(), input_parameters.begin());
      for (const auto detector : {ND, FDI, FDII}) {
        using enum params::dc::Detector;
        for (int i = NuShape01; i <= NuShape43; ++i) {
          input_parameters[params::index(detector, i)] = 0.0;
        }
      }
      input_parameters[SinSqT13] = 0.0;
      {
        dc_llh->check_and_recalculate(input_parameters.data());

        for (const auto detector : detector_types) {
          const span_t sig = signal.get_spectrum(detector);

          const double mcNorm = dc_llh->calculate_mcNorm(parameter, detector);

          std::transform(sig.begin(),
                         sig.end(),
                         signal_prediction.begin(),
                         [mcNorm](double x) -> double {
                           return x * mcNorm;
                         });

          std::string name = params::dc::get_detector_name(detector);

          j[name]["signal0-shape"] = signal_prediction;
        }
      }
      return j;
    }

    inline void write_double_chooz_results(ana::Fit& fit, std::string_view name) {
      auto j = get_json_file(fit);

      std::stringstream ss;
      ss << name << ".json";
      std::ofstream file;
      file.open(ss.str());
      file << j.dump() << '\n';
      file.close();
    }
  }  // namespace dc

  inline void write_results(ana::Fit& fit, std::string_view name) {
    if (fit.use_double_chooz()) {
      dc::write_double_chooz_results(fit, name);
    } else {
      throw std::invalid_argument("Only Double Chooz results are supported at the moment.");
    }
  }

}  // namespace result
