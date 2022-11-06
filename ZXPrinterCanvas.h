#pragma once

template<int ROWS = 8>
class ZXPrinterCanvas {
private:
  static const int rows = ROWS;
  static const int columns = 256;

  byte pixels[rows*(columns/8)] = {};

  byte* pixelptr = (byte*) pixels;
  byte pixelmask = 0x80;

  inline int makeindex(int row, int column) __attribute__((always_inline)) {
    return row*(columns/8)+column/8;
  }

  inline byte makemask(int column) __attribute__((always_inline)) {
    return 0x80>>(column&7);
  }

  void nextpixel() {
    pixelmask >>= 1;
    if(!pixelmask) {
      pixelmask = 0x80;
      if (pixelptr) ++pixelptr;
    }
  }

public:
  inline const int getRows() {
    return rows;
  }

  inline const int getColumns() {
    return columns;
  }

  void drawPixel(const int row, const int column, const bool state) {
    if (row<0 || column<0 || row>=rows || column>=columns) return;
    int index = makeindex(row, column);
    byte mask = makemask(column);
    if (state)
      pixels[index] |= mask;
    else
      pixels[index] &= ~mask;
  }

  void clear() {
    memset(pixels, 0, sizeof(pixels));
  }

  void seekPixel(const int row = 0, const int column = 0) {
    int index = makeindex(row, column);
    if (index >= sizeof(pixels)) {
      pixelptr = 0;
    } else {
      pixelptr = pixels + index;
    }
    pixelmask = makemask(column);
  }

  void skipPixel(int count) {
    while(count--) nextpixel();
  }

  bool readPixel() {
    bool ispixel = pixelptr ? *pixelptr & pixelmask : false;
    nextpixel();
    return ispixel;
  }
};
