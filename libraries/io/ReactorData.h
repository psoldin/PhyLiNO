#pragma once

// includes
#include "Parameter.h"
#include "TreeEntry.h"

// STL includes
#include <span>
#include <vector>

namespace io {

  /**
   * @brief Class representing reactor data.
   *
   * This class contains reactor data, including energy, scaling, and distance information.
   */
  class ReactorData {
   public:
    /**
     * @brief Constructor for ReactorData.
     *
     * @param entries A span of TreeEntry objects.
     * @param type The detector type.
     */
    explicit ReactorData(std::span<TreeEntry> entries, params::dc::DetectorType type);

    /**
     * @brief Default destructor for ReactorData.
     */
    ~ReactorData() = default;

    /**
     * @brief Returns a span of energy values.
     *
     * @return A span of energy values.
     */
    [[nodiscard]] std::span<const double> evis() const noexcept { return m_Evis; }

    /**
     * @brief Returns a span of true energy values.
     *
     * @return A span of true energy values.
     */
    [[nodiscard]] std::span<const double> etrue() const noexcept { return m_Etrue; }

    /**
     * @brief Returns a span of scaling values.
     *
     * @return A span of scaling values.
     */
    [[nodiscard]] std::span<const double> scaling() const noexcept { return m_Scaling; }

    /**
     * @brief Returns a span of L/E values.
     *
     * @return A span of L/E values.
     */
    [[nodiscard]] std::span<const double> LoverE() const noexcept { return m_LoverE; }

    /**
     * @brief Returns a span of distance values.
     *
     * @return A span of distance values.
     */
    [[nodiscard]] std::span<const double> distance() const noexcept { return m_Distance; }

    /**
     * @brief Returns the detector type.
     *
     * @return The detector type.
     */
    [[nodiscard]] params::dc::DetectorType detectorType() const noexcept { return m_DetectorType; }

   private:
    params::dc::DetectorType m_DetectorType; /**< The detector type. */
    std::vector<double>      m_Evis;         /**< A vector of energy values. */
    std::vector<double>      m_Etrue;        /**< A vector of true energy values. */
    std::vector<double>      m_Scaling;      /**< A vector of scaling values. */
    std::vector<double>      m_LoverE;       /**< A vector of L/E values. */
    std::vector<double>      m_Distance;     /**< A vector of distance values. */
  };
}  // namespace io