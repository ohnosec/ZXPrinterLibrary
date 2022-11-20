#pragma once

#include "ZXPrinterDriver.h"
#include "ZXPrinterSerial.h"
#include "ZXPrinterCanvas.h"
#include "ZXPrinterDraw.h"
#include "ZXPrinterPrint.h"

template<int ROWS>
class ZXPrinterBase:
  public ZXPrinterShield<ZXPrinterBase<ROWS>>,
  public ZXPrinterSerial<ZXPrinterBase<ROWS>>,
  public ZXPrinterCanvas<ROWS>,
  public ZXPrinterDraw<ZXPrinterBase<ROWS>>,
  public ZXPrinterPrint<ZXPrinterBase<ROWS>>
{
};

class ZXPrinter: public ZXPrinterBase<8> {};
