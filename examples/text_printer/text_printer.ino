#include <ZXPrinter.h>
#include <Fonts/ZXPrinterFont5x7.h>

/*
Emulates a serial text printer.

The printer emulation uses the Arduino serial port, but not directly, you must use a usb->serial adapter.

This example has been tested on Windows 10 with an FTDI usb->serial adapter.

As the printer is quite slow it must pause the incoming serial data while it prints a line.
This pausing is called flow control. If we didn't flow control then some printing would be lost.
There are two types of flow control, hardware and software. Unfortunately, the Arduino serial driver
doesn't support either type of flow control.

NOTE: Not all usb->serial adapters support flow control, and some only hardware flow control.

Steps to using this example:

1. plug the usb->serial adapter into the serial header of the ZXPS shield
   make sure you connect the adapter in the right way up, lining up the tx->rx and rx->tx pins
2. connect the usb from the adapter to your computer taking note of the serial port of the adapter
3. configure a serial text printer on the serial port, for example in Windows 10
   in "Printers & Scanners"
     - click "Add a printer or scanner"
     - click "The printer that I want isn't listed"
     - select "Add a local printer or network printer with manual settings" then click "Next"
     - click "Use an existing port"
     - select the serial port with the usb->serial adapter then click "Next"
     - select "Generic"->"Generic / Text Only" then click "Next"
     - select "Use the driver that is currently installed" then click "Next"
     - enter a "Printer name" then click "Next"
     - share the printer if you like
     - click "Finish"
     - in "Printers & Scanners" select the new printer then click "Manage"
     - click "Printer properties"
     - click the "Ports" tab
     - click "Configure Port"
     - for "Bits per second" select "9600"
     - for "Flow control" select "Xon / Xoff"
     - click "OK"

NOTE: The configuration steps above assume software flow control (XON/XOFF), but the code also
supports hardware flow control (RTS/CTS). However, in the Windows port configuration, hardware
flow control requires both CTS and DSR. So, if you want to use hardware flow control you will
need to connect these two control signals together on your usb->serial adapter. This may not
be possible as not all usb->serial adapters break out both the CTS and the DSR control signals.
*/

ZXPrinter printer;

void setup() {
  Serial.begin(9600);

  printer.begin();
  printer.setFont(&ZXPrinterFont5x7);
}

void loop() {
  if (Serial.available() > 0) {
    int ch = Serial.read();
    if (ch == printer.newline) printer.pauseSerial();
    printer.print((char) ch);
    if (ch == printer.newline) printer.resumeSerial();
  }
}
