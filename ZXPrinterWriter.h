#pragma once

#include "ZXPrinterFont.h"
#include "crtp.h"

template<typename T>
class ZXPrinterWriter :  public CRTP<T, ZXPrinterWriter> {
  using CRTP<T, ZXPrinterWriter>::impl;

  int cursorcolumn = 0;
  int charheight = 0;
  int charwidth = 0;
  int chargap = 0;
  byte* pbitmap = NULL;

  byte getfontmask(char ch, byte row) {
    if (ch<' ' || ch>127) return 0;
    if (!pbitmap) return 0;
    byte offset = ch-' ';
    return pgm_read_byte(pbitmap+offset*charheight+row);
  }

public:
  void setCursor(int column = 0) {
    cursorcolumn = column;
  }

  void homeCursor() {
    setCursor();
  }

  void setFont(const ZXPrinterFont *font) {
    charheight = pgm_read_byte(&font->charheight);
    charwidth = pgm_read_byte(&font->charwidth);
    chargap = pgm_read_byte(&font->chargap);
    pbitmap = (byte*) pgm_read_word(&font->bitmap);
  }

  void drawChar(char ch, int column = -1) {
    if (column<0) column = cursorcolumn;
    byte bitmask = 1<<(charwidth-1);
    for(int row=0; row<charheight; row++) {
      for(int bitcount=0; bitcount<charwidth; bitcount++) {
        bool isset = getfontmask(ch, row) & (bitmask >> bitcount);
        impl().drawPixel(row, column+bitcount, isset);
      }
    }
    cursorcolumn += charwidth+chargap;
  }

  void drawText(char* text, int column = 0) {
    cursorcolumn = column;
    while(char ch = *text++) {
      drawChar(ch);
    }
  }

  void drawBitmap(const uint8_t bitmap[], int column, int width, int height) {
    byte* pbitmap = bitmap;
    byte row;
    for(int y=0; row=y%impl().getRows(), y<height; y++) {
      byte bitmask;
      byte bitpattern;
      for(int x=0; x<width; x++) {
        if (x%8 == 0) {
          bitmask = 1<<7;
          bitpattern = pgm_read_byte(pbitmap++);
        }
        if (bitpattern & bitmask) {
          impl().drawPixel(row, column+x, true);
        }
        bitmask >>= 1;
      }
      if (row==impl().getRows()-1) {
        impl().printBuffer();
        impl().clear();
      }
    }
    if (row) {
      impl().printBuffer(row);
      impl().clear();
    }
  }
};
