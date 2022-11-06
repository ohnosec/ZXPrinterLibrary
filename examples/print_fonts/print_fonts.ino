#include <ZXPrinter.h>
#include <Fonts/ZXPrinterFont5x7.h>
#include <Fonts/ZXPrinterFontTS2068.h>
#include <Fonts/ZXPrinterFontAmstradCPC.h>

ZXPrinter printer;

void setup() {
  printer.begin();

  printer.setFont(&ZXPrinterFont5x7);
  printer.println("This is the 5x7 font");
  printer.println();

  printer.setFont(&ZXPrinterFontTS2068);
  printer.println("This is the TS 2068 font");
  printer.println();

  printer.setFont(&ZXPrinterFontAmstradCPC);
  printer.println("This is the Amstrad CPC font");
  printer.println();
}

void loop() {
}
