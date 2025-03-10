#pragma once

namespace rslt {

  void profile_likelihood_scan(boost::asio::thread_pool& pool,
                               const io::Options& options,
                               const std::string& output_dir,
                               unsigned int n_points,
                               std::function<void(Analysis::Fitter&)> function,
                               double min,
                               double max) {
    // Create the output directory
    boost::filesystem::create_directories(output_dir);

    // Create the fitter object
    Analysis::Fitter fit(options);

    // Perform the likelihood scan
    fit.perform_likelihood_scan(pool, output_dir, n_points, function, min, max);

  }