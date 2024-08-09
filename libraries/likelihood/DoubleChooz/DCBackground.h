#pragma once

#include "../BackgroundBase.h"

namespace ana::dc {

  template <typename... Backgrounds>
  class DCBackground : public BackgroundBase<DCBackground<Backgrounds...>> {
   public:
    template <typename... Args>
    explicit DCBackground(Args&&... args)
        : m_Backgrounds(Backgrounds(std::forward<Args>(args)...)...) { }

    template <typename T>
    const T& get_background() const noexcept {
      return std::get<T>(m_Backgrounds);
    }

   private:
    std::tuple<Backgrounds...> m_Backgrounds;
  };

}