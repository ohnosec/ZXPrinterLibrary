#pragma once

#include <Arduino.h>

typedef struct {
  byte charheight;
  byte charwidth;
  byte chargap;
  byte* bitmap;
} ZXPrinterFont;
