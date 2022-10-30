#include <ZXPrinter.h>
#include <Fonts/ZXPrinterFont5x7.h>

ZXPrinter printer;

void setup() {
  printer.begin();
  
  printer.setFont(&ZXPrinterFont5x7);
  
  printer.clear();
  printer.println("Hello world!");
}

void loop() {
}
