#pragma once

#include <Arduino.h>

typedef struct {
  const byte charheight;
  const byte charwidth;
  const byte chargap;
  const byte linegap;
  const byte* bitmap;
} ZXPrinterFont;
