#pragma once

#include "Options.h"

namespace ana {

/**
 * @brief Represents an Oscillator.
 * 
 * This class provides functionality to handle an Oscillator.
 */
class Oscillator {
 public:
    /**
     * @brief Constructs an Oscillator object with the given options.
     * 
     * @param options The options for the Oscillator.
     */
    explicit Oscillator(const io::Options& options);

    /**
     * @brief Destructor for the Oscillator object.
     */
    ~Oscillator() = default;

    void recalculate_spectra();

 private:
    const io::Options& m_Options; /**< The options for the Oscillator. */
};
}  // namespace ana