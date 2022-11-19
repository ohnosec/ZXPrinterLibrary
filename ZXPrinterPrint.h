#pragma once

#include "CRTP.h"

template<typename T>
class ZXPrinterPrint : public CRTP<T, ZXPrinterPrint>, public Print {
  using CRTP<T, ZXPrinterPrint>::self;

  const char carriagereturn = '\r';

public:
  char newline = '\n';

  virtual size_t write(uint8_t c) {
    if (c == newline) {
      flush(self().getCharHeight());
      rowFeed(self().getLineGap());
      self().homeCursor();
    } else if (c == carriagereturn) {
      self().homeCursor();
    } else {
      self().drawChar(c);
    }
    return 1;
  }

  void printBitmap(const uint8_t bitmap[], const int column, const int width, const int height, const bool invert = false) {
    const byte* pbitmap = bitmap;
    const int rows = self().getRows();
    int row = 0;
    for(int y=0; row=y%rows, y<height; y++) {
      byte bitmask = 0;
      byte bitpattern = 0;
      for(int x=0; x<width; x++) {
        if (bitmask == 0) {
          bitmask = 1<<7;
          bitpattern = pgm_read_byte(pbitmap++);
        }
        bool pixel = bitpattern & bitmask;
        if (invert) pixel = !pixel;
        self().drawPixel(row, column+x, pixel);
        bitmask >>= 1;
      }
      if (row == rows-1) flush();
    }
    if (row) flush(row);
  }

  void flush(const int rows = -1) {
    self().printCanvas(rows);
    self().clear();
  }

  void rowFeed(const int rows = 1) {
    self().printCanvas(rows, false);
  }

  void lineFeed() {
    self().printCanvas(self().getLineHeight(), false);
  }

  void formFeed() {
    self().printCanvas(self().getRows(), false);
  }
};
