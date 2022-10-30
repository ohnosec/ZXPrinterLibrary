#pragma once

#include <digitalWriteFast.h>
#include "CRTP.h"

#ifndef ZXPSVERSION
  #define ZXPSVERSION 2
#endif

template<
  typename T,
  byte A2, byte A7,
  byte IORQ, byte WR, byte RD,
  byte D0, byte D1, byte D2, byte D3, byte D4, byte D5, byte D6, byte D7
>
class ZXPrinterDriver : public CRTP<T, ZXPrinterDriver> {
  using CRTP<T, ZXPrinterDriver>::impl;

  // Read data
  const byte IN_PRINTER_READY = D0;
  const byte IN_PRINTER_NOT_DETECTED = D6;
  const byte IN_PRINTER_ON_PAPER = D7;

  // Write data
  const byte OUT_PRINTER_MOTOR_SLOW = D1;
  const byte OUT_PRINTER_MOTOR_OFF = D2;
  const byte OUT_PRINTER_PIXEL_ON = D7;

  inline void readon() {
    digitalWriteFast(RD, LOW);
  }

  inline void readoff() {
    digitalWriteFast(RD, HIGH);
  }

  inline void writeon() {
    digitalWriteFast(WR, LOW);
  }

  inline void writeoff() {
    digitalWriteFast(WR, HIGH);
  }

  inline void readtoggle() {
    readon();
    readoff();
  }

  inline void writetoggle() {
    writeon();
    writeoff();
  }

  inline bool isready() {
    readon();
    bool value = digitalReadFast(IN_PRINTER_READY)==HIGH;
    readoff();
    return value;
  }

  inline bool isonpaper() {
    readon();
    bool value = digitalReadFast(IN_PRINTER_ON_PAPER)==HIGH;
    readoff();
    return value;
  }

  inline bool isdetected() {
    readon();
    bool value = digitalReadFast(IN_PRINTER_NOT_DETECTED)==LOW;
    readoff();
    return value;
  }

  inline void outpixel(bool ison) {
    if(ison) {
      digitalWriteFast(OUT_PRINTER_PIXEL_ON, HIGH);
    } else {
      digitalWriteFast(OUT_PRINTER_PIXEL_ON, LOW);
    }
    _delay_us(2); // needs to be stable for a couple of microseconds, not sure why
    pinModeFast(OUT_PRINTER_PIXEL_ON, OUTPUT);
    writetoggle();
    pinModeFast(OUT_PRINTER_PIXEL_ON, INPUT);
    digitalWriteFast(OUT_PRINTER_PIXEL_ON, LOW);
  }

  inline void reset() {
    outpixel(false); // reset latches
  }

  inline void waitforpaper() {
    outpixel(false); // paper detect doesn't work while stylus is on
    while(!isonpaper());
  }

  inline void waitfornopaper() {
    outpixel(false); // paper detect doesn't work while stylus is on
    while(isonpaper());
  }

  inline void motoron() {
    digitalWriteFast(OUT_PRINTER_MOTOR_OFF, LOW);
    writetoggle();
  }

  inline void motoroff() {
    digitalWriteFast(OUT_PRINTER_MOTOR_OFF, HIGH);
    writetoggle();
  }

  inline void motorslow() {
    digitalWriteFast(OUT_PRINTER_MOTOR_SLOW, HIGH);
    writetoggle();
  }

  inline void motorfast() {
    digitalWriteFast(OUT_PRINTER_MOTOR_SLOW, LOW);
    writetoggle();
  }

  void motor(bool state) {
    if (state) motoron();
    else motoroff();
  }
public:
  void begin() {
    #if ZXPSVERSION < 2
      digitalWriteFast(A2, LOW);
      digitalWriteFast(A7, HIGH);
      digitalWriteFast(IORQ, LOW);
    #endif
    digitalWriteFast(WR, HIGH);
    digitalWriteFast(RD, HIGH);

    #if ZXPSVERSION < 2
      pinModeFast(A2, OUTPUT);
      pinModeFast(A7, OUTPUT);
      pinModeFast(IORQ, OUTPUT);
    #endif
    pinModeFast(WR, OUTPUT);
    pinModeFast(RD, OUTPUT);

    digitalWriteFast(IN_PRINTER_READY, LOW);
    digitalWriteFast(IN_PRINTER_NOT_DETECTED, LOW);
    digitalWriteFast(IN_PRINTER_ON_PAPER, LOW);
    digitalWriteFast(OUT_PRINTER_MOTOR_SLOW, HIGH);
    digitalWriteFast(OUT_PRINTER_MOTOR_OFF, HIGH);

    pinModeFast(IN_PRINTER_READY, INPUT);
    pinModeFast(IN_PRINTER_NOT_DETECTED, INPUT);
    pinModeFast(IN_PRINTER_ON_PAPER, INPUT);
    pinModeFast(OUT_PRINTER_MOTOR_SLOW, OUTPUT);
    pinModeFast(OUT_PRINTER_MOTOR_OFF, OUTPUT);
  }

  bool isConnected() {
    return isdetected();
  }

  bool printBuffer() {
    outpixel(false);
    if (!isdetected()) {
      return false;
    }
    motoron();
    motorslow();
    //TODO: we should start out fast and slow down for the last two lines
    //motorfast();
    waitfornopaper();
    impl().seekPixel();
    for (byte row = 0; row < impl().getRows(); row++) {
      waitforpaper();
      bool pixelon = false;
      for (byte column = 0; column < impl().getColumns()-1; column++) {
        outpixel(pixelon);
        pixelon = impl().readPixel();
        while(!isready());
        outpixel(pixelon);
       }
      impl().skipPixel(1);
      outpixel(false);
    }
    readtoggle();
    motoroff();
    return true;
  }
};

#if ZXPSVERSION < 2
  #if defined(__AVR_ATmega2560__)
    template<typename T> using ZXPrinterShield = ZXPrinterDriver<T, 21, 28, 38, 36, 37, 49, 48, 47, 44, 43, 45, 46, A13>;
  #elif defined(__AVR_ATmega328P__)
    template<typename T> using ZXPrinterShield = ZXPrinterDriver<T, A0, A1, 10, 12, 11, 2, 3, 4, 5, 6, 7, 8, 9>;
  #else
    #error Unsupported board
  #endif
#else
  #if defined(__AVR_ATmega328P__)
    template<typename T> using ZXPrinterShield = ZXPrinterDriver<T, -1, -1, -1, 9, 8, 3, 4, 5, -1, -1, -1, 6, 7>;
  #else
    #error Unsupported board
  #endif
#endif
