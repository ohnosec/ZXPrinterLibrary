#include <ZXPrinter.h>
#include <Fonts/ZXPrinterFont5x7.h>

ZXPrinter printer;

void setup() {
  printer.begin();
  // NOTE: if you don't set the font then nothing will be printed
  printer.setFont(&ZXPrinterFont5x7);

  printer.println("Hello world!");
}

void loop() {
}
