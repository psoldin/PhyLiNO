#pragma once

#include "Options.h"

/**
 * @brief The BackgroundBase class is a base class for background models in the ana namespace.
 *
 * It provides a common interface for derived classes to implement background models.
 *
 * @tparam Derived The derived class type.
 */
namespace ana {

  template <typename Derived>
  class BackgroundBase {
   public:
    /**
     * @class BackgroundBase
     * @brief Base class for background calculations.
     *
     * This class provides a base implementation for background calculations in the DoubleChooz experiment.
     * It is intended to be inherited by specific background classes that implement the actual calculations.
     * The class takes an options object as a parameter in its constructor.
     */
    explicit BackgroundBase(std::shared_ptr<io::Options>&& options)
        : BackgroundBase(std::move(options)) { }

    /**
     * @brief Destructor for the BackgroundBase class.
     */
    virtual ~BackgroundBase() = default;

    /**
     * @brief Get the options object.
     *
     * @return const std::shared_ptr<io::Options>& The options object.
     */
    [[nodiscard]] const std::shared_ptr<io::Options>& options() const noexcept { return m_Options; }

   protected:
    std::shared_ptr<io::Options> m_Options;
  };

}