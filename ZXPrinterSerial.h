#pragma once

template<typename T>
class ZXPrinterSerial : public CRTP<T, ZXPrinterSerial> {
  using CRTP<T, ZXPrinterSerial>::self;

private:
  const byte XON = 0x11;
  const byte XOFF = 0x13;
  
public:
  void pauseSerial() {
    Serial.write(XOFF);
    self().ctsOff();
  }

  void resumeSerial() {
    Serial.write(XON);
    self().ctsOn();
  }
};
