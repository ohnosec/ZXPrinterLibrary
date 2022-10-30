#pragma once

#include "CRTP.h"

template<typename T>
class ZXPrinterPrint : public CRTP<T, ZXPrinterPrint>, public Print {
  using CRTP<T, ZXPrinterPrint>::impl;
public:
  virtual size_t write(uint8_t c) {
    if (c == '\n') {
      impl().homeCursor();
      impl().printBuffer();
      impl().clear();
    } else if (c != '\r') {
      impl().drawChar(c);
    }
    return 1;
  }
};
