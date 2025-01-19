#include <ZXPrinter.h>
#include <Fonts/ZXPrinterFont5x7.h>

/*
Emulates a serial text printer so you can print from a modern Operating System.

This example has been tested on:
  - Windows 10 using an FTDI usb->serial adapter
  - Raspberry Pi 400 running Raspian Bullseye using serial on the GPIO connector

Flow control:

As printing is quite slow the Arduino must occasionally pause the incoming serial data while 
it prints a line. This pausing is called flow control. If we didn't flow control then some 
printout would be lost. There are two types of flow control, hardware and software.

NOTE: 
  - You can't use the Arduino USB serial port as the serial driver doesn't support flow control
  - Not all usb->serial adapters support flow control, and some only support hardware flow control

Steps to using this example:

On Windows:

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

On Raspberry Pi:

1. enable the Serial Port and disable the Serial Console. On the desktop...
   - click top left Raspberry icon->Preferences->Raspberry Pi Configuration->Interfaces
   - reboot
2. connect the Raspberry Pi to the ZXPS shield.
   - connect pin 6 (Ground) to the ZXPS shield serial header GND
   - connect pin 8 (UART TXD) to the ZXPS shield serial header RX
   - connect pin 10 (UART RXD) to the ZXPS shield serial header TX through a voltage divider
                   _____            _____
     ZXPS TX -----|_____|----o-----|_____|------ GND
                    10K      |       10K
     Pi pin 10 --------------+
3. add a text printer. On the desktop...
   - click top left Raspberry icon->Preferences->Print Settings
   - click "Add"
   - click "Serial Port #1"
   - for "Baud Rate" select "9600"
   - for "Flow Control" select "XON/XOFF (Software)"
   - click "Forward"
   - select "Generic (recommended)"
   - click "Forward"
   - select "Text-Only (recommended)"
   - click "Forward"
   - disable "Send Form Feed after each page"
   - click "Forward"
   - enter a "Printer Name" and "Description"  
   - click "Apply"
   - if the new printer doesn't have a geen tick, right click the printer and "Set As Default" 
   - right click the printer->Properties
   - select "Printer Options"
   - set the "Left Margin", "Right Margin", "Top Margin", and "Bottom Margin" to 0
   - Click "OK"
4. to print a text file. In a terminal...
   - Enter "lp [filename]" where [filename] is the text file to print
*/

ZXPrinter printer;

const unsigned int BUFSIZE = 512;
char buf[BUFSIZE];
unsigned int bufpos;

const unsigned long IDLETIME = 1000;
unsigned long lastprinted;

const byte END_OF_PRINT_LINES = 3;

bool isprinting;

void setup() {
  Serial.begin(9600);

  printer.begin();
  printer.setFont(&ZXPrinterFont5x7);
  printer.resumeSerial();

  bufpos = 0;
  isprinting = false;
  lastprinted = millis();
}

void printbuffer() {
  if (bufpos>0) {
    printer.pauseSerial();
    for(int i=0; i<bufpos; i++) {
      printer.print(buf[i]);
    }
    printer.resumeSerial();
    bufpos = 0;
  }
}

void loop() {
  if (Serial.available() > 0) {
    isprinting = true;
    int ch = Serial.read();
    buf[bufpos++] = ch;
    if (bufpos >= BUFSIZE) {
      printbuffer();
    }
    lastprinted = millis();
  }
  if (isprinting && (millis()-lastprinted >= IDLETIME)) {
    isprinting = false;
    printbuffer();
    for(int i=0; i<END_OF_PRINT_LINES; i++) {
      printer.println();
    }
  }
}