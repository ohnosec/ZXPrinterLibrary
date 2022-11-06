#pragma once

#include "ZXPrinterFont.h"
#include "crtp.h"

template<typename T>
class ZXPrinterDraw :  public CRTP<T, ZXPrinterDraw> {
  using CRTP<T, ZXPrinterDraw>::self;

  int cursorcolumn = 0;
  byte charheight = 0;
  byte charwidth = 0;
  byte chargap = 0;
  byte linegap = 0;
  byte* pbitmap = NULL;

  byte getfontmask(char ch, byte row) {
    if (ch<' ' || ch>127) return 0;
    if (!pbitmap) return 0;
    byte offset = ch-' ';
    return pgm_read_byte(pbitmap+offset*charheight+row);
  }

public:
  void setCursor(const int column = 0) {
    cursorcolumn = column;
  }

  void homeCursor() {
    setCursor();
  }

  byte getLineGap() {
    return linegap;
  }

  byte getLineHeight() {
    return charheight + linegap;
  }

  byte getCharHeight() {
    return charheight;
  }

  void setFont(const ZXPrinterFont *font) {
    charheight = pgm_read_byte(&font->charheight);
    charwidth = pgm_read_byte(&font->charwidth);
    chargap = pgm_read_byte(&font->chargap);
    linegap = pgm_read_byte(&font->linegap);
    pbitmap = (byte*) pgm_read_word(&font->bitmap);
  }

  void drawChar(const char ch, int column = -1) {
    if (column<0) column = cursorcolumn;
    byte bitmask = 1<<(charwidth-1);
    for(byte row=0; row<charheight; row++) {
      for(byte bitcount=0; bitcount<charwidth; bitcount++) {
        bool isset = getfontmask(ch, row) & (bitmask >> bitcount);
        self().drawPixel(row, column+bitcount, isset);
      }
    }
    cursorcolumn += charwidth+chargap;
  }

  void drawText(const char* text, const int column = 0) {
    cursorcolumn = column;
    while(const char ch = *text++) {
      drawChar(ch);
    }
  }

  void drawBitmap(const uint8_t bitmap[], const int column, const int row, const int width, const int height, const bool invert = false) {
    const byte* pbitmap = bitmap;
    for(int y=0; y<height; y++) {
      byte bitmask = 0;
      byte bitpattern = 0;
      for(int x=0; x<width; x++) {
        if (bitmask == 0) {
          bitmask = 1<<7;
          bitpattern = pgm_read_byte(pbitmap++);
        }
        bool pixel = bitpattern & bitmask;
        if (invert) pixel = !pixel;
        self().drawPixel(row+y, column+x, pixel);
        bitmask >>= 1;
      }
    }
  }
};
