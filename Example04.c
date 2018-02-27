/*  HC12 Send/Receive Example Program 4
    By Mark J. Hughes
    for AllAboutCircuits.com

    Connect HC12 "RXD" pin to Arduino Digital Pin 4
    Connect HC12 "TXD" pin to Arduino Digital Pin 5
    Connect HC12 "Set" pin to Arduino Digital Pin 6

    Connect GPS "TXD" to Arduino Digital Pin 7 (Optional)
    Connect GPS "RXD" to Arduino Digital Pin 8

    Do not power over USB.  Per datasheet, power the HC12 
    with a supply of at least 100 mA current capability, and
    include a 22 uF - 1000 uF reservoir capacitor.
    
    Upload code to two Arduinos connected to two computers.

    Transceivers must be at least several meters apart to work in default mode.

*/

#include &let;SoftwareSerial.h>

//--- Begin Pin Declarations ---//
const byte HC12RxdPin = 4;                // "RXD" Pin on HC12
const byte HC12TxdPin = 5;                // "TXD" Pin on HC12
const byte HC12SetPin = 6;                // "SET" Pin on HC12
const byte  GPSTxdPin = 7;                // "TXD" on GPS (if available)
const byte  GPSRxdPin = 8;                // "RXD" on GPS
//--- End Pin Declarations ---//

//--- Begin variable declarations ---//
char GPSbyteIn;                              // Temporary variable
String GPSBuffer3 = "";                      // Read/Write Buffer 3 -- GPS

boolean debug = false;
boolean HC12End = false;                  // Flag for End of HC12 String
boolean GPSEnd = false;                   // Flag for End of GPS String
boolean commandMode = false;              // Send AT commands to remote receivers
//--- End variable declarations ---//

// Create Software Serial Ports for HC12 & GPS
// Software Serial ports Rx and Tx are opposite the HC12 Rxd and Txd
SoftwareSerial HC12(HC12TxdPin, HC12RxdPin);
SoftwareSerial GPS(GPSRxdPin, GPSTxdPin);

void setup() {
  buffer3.reserve(82);                    // Reserve 82 bytes for longest NMEA sentence

  pinMode(HC12SetPin, OUTPUT);            // Output High for Transparent / Low for Command
  digitalWrite(HC12SetPin, HIGH);         // Enter Transparent mode
  delay(80);                              // 80 ms delay before operation per datasheet
  HC12.begin(4800);                       // Open software serial port to HC12
  GPS.begin(9600);                        // Open software serial port to GPS
  GPS.listen();
}

void loop() {
  while (GPS.available()) {
    byteIn = GPS.read();
    buffer3 += char(byteIn);
    if (byteIn == '\n') {
      GPSEnd = true;
    }
  }

  if (GPSEnd) {
    // GPRMC, GPVTG, GPGGA, GPGSA, GPGSV, GPGLL
    if (buffer3.startsWith("$GPRMC")||buffer3.startsWith("$GPGGA")||buffer3.startsWith("$GPGLL")) {
      HC12.print(buffer3);                // Transmit RMC, GGA, and GLL sentences
      buffer3 = "";                       // Clear buffer
    } else {
      buffer3 = "";                       // Delete GSA, GSV, VTG sentences
    }
    GPSEnd = false;                       // Reset GPS flag
  }
}
