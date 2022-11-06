#pragma once

template<typename T, template<typename, byte...> class CRTPT>
struct CRTP {
  T& self() { return static_cast<T&>(*this); }
  T const &self() const { return static_cast<T const &>(*this); }
};
