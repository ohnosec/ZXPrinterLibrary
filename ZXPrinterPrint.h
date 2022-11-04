#pragma once

#include "CRTP.h"

template<typename T>
class ZXPrinterPrint : public CRTP<T, ZXPrinterPrint>, public Print {
  using CRTP<T, ZXPrinterPrint>::self;
public:
  virtual size_t write(uint8_t c) {
    if (c == '\n') {
      self().homeCursor();
      self().printCanvas();
      self().clear();
    } else if (c != '\r') {
      self().drawChar(c);
    }
    return 1;
  }

  void printBitmap(const uint8_t bitmap[], int column, int width, int height) {
    byte* pbitmap = bitmap;
    byte row;
    byte rows = self().getRows();
    for(int y=0; row=y%rows, y<height; y++) {
      byte bitmask;
      byte bitpattern;
      for(int x=0; x<width; x++) {
        if (x%8 == 0) { 
          bitmask = 1<<7;
          bitpattern = pgm_read_byte(pbitmap++);
        }
        if (bitpattern & bitmask) {
          self().drawPixel(row, column+x, true);
        }
        bitmask >>= 1;
      }
      if (row == rows-1) {
        self().printCanvas();
        self().clear();
      }
    }
    if (row) {
      self().printCanvas(row);
      self().clear();
    }
  }
};
