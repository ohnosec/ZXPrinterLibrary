#pragma once

template<typename T, template<typename, byte...> class CRTPT>
struct CRTP { 
  T& impl() { return static_cast<T&>(*this); } 
  T const &impl() const { return static_cast<T const &>(*this); } 
};
