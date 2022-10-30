#pragma once

#include "ZXPrinterDriver.h"
#include "ZXPrinterBuffer.h"
#include "ZXPrinterWriter.h"
#include "ZXPrinterPrint.h"

template<int ROWS>
class ZXPrinterBase:
  public ZXPrinterShield<ZXPrinterBase<ROWS>>,
  public ZXPrinterBuffer<ROWS>,
  public ZXPrinterWriter<ZXPrinterBase<ROWS>>,
  public ZXPrinterPrint<ZXPrinterBase<ROWS>>
{
};

class ZXPrinter: public ZXPrinterBase<8> {};