#pragma once

template<int ROWS = 8>
class ZXPrinterCanvas {
private:
  static const int rows = ROWS;
  static const int columns = 256;

  byte pixels[rows*(columns/8)] = {};
  
  byte* pixelptr = (byte*) pixels;
  byte pixelmask = 0x80;
    
  inline byte makeindex(int row, int column) __attribute__((always_inline)) {
    return row*(columns/8)+column/8;
  }
  
  inline byte makemask(int column) __attribute__((always_inline)) {
    return 0x80>>(column&7);
  }

  void nextpixel() {
    pixelmask >>= 1;
    if(!pixelmask) {
      pixelmask = 0x80;
      ++pixelptr;
    }
  }
public:
  inline const int getRows() {
    return rows;
  }

  inline const int getColumns() {
    return columns;
  }

  void drawPixel(int row, int column, bool state) {
    if (row<0 || column<0 || row>=rows || column>=columns) return;
    byte index = makeindex(row, column);
    byte mask = makemask(column);
    if (state)
      pixels[index] |= mask;
    else
      pixels[index] &= ~mask;
  }
  
  void clear() {
    memset(pixels, 0, sizeof(pixels));
  }

  void seekPixel(int row = 0, int column = 0) {
    pixelptr = (byte*)pixels+makeindex(row, column);
    pixelmask = makemask(column);
  }
    
  void skipPixel(int count) {
    while(count--) nextpixel();
  }
  
  bool readPixel() {
    bool ispixel = *pixelptr & pixelmask;
    nextpixel();
    return ispixel;
  }
};
