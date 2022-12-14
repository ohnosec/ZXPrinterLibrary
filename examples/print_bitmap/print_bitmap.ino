#include <ZXPrinter.h>
#include "bitmap.h"

/*
Prints a bitmap from the bitmap.h file.

The bitmap must be stored in program memory (PROGMEM). See the bitmap.h file for an example.

There is a utility that will convert monochrome bitmaps into the correct format at
http://javl.github.io/image2cpp/

To convert a bitmap file into the correct format:

- create a monochrome bitmap file up to 256 pixels wide
- browse the above address
  - in step 1 click "Choose Files" to upload the bitmap file
  - in step 2 check "Invert image colors"
  - in step 2 note the "Canvas size", this is the width and the height
  - in step 4 click the "Generate code" and then "Copy output"
  - in step 4 note the bitmap name
- paste the code into the bitmaps.h file
- update below with drawBitmap([bitmap name], 0, [canvas width], [canvas height]);
- the second parameter of drawBitmap is the horizontal position of the bitmap
*/

ZXPrinter printer;

void setup() {
  printer.begin();

  printer.printBitmap(logo, 0, 256, 72);
}

void loop() {
}
