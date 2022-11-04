#pragma once

#include "ZXPrinterFont.h"
#include "crtp.h"

template<typename T>
class ZXPrinterDraw :  public CRTP<T, ZXPrinterDraw> {
  using CRTP<T, ZXPrinterDraw>::self;

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
        self().drawPixel(row, column+bitcount, isset);
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
};
