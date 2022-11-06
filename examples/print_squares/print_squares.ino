#include <ZXPrinter.h>

ZXPrinter printer;

void setup() {
  printer.begin();

  printer.clear();
  drawSquares();
  printer.flush();
}

void drawSquares() {
  for(int row = 0; row < printer.getRows(); row++) {
    bool pixel = false;
    for (int column = 0; column < printer.getColumns(); column++) {
      if ((column % 8) == 0) pixel = !pixel;
      printer.drawPixel(row, column, pixel);
    }
  }
}

void loop() {
}
