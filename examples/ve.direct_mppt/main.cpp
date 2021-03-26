#include <Arduino.h>
#include <SoftwareSerial.h>  // Need software serial for chargers 3-6
#include "sensesp_app.h"
#include "vedirect_helper.h"  // Need the companion helper app
#include "sensesp_app_builder.h"

ReactESP app([]() {
  SensESPAppBuilder builder;
  sensesp_app = builder.set_standard_sensors(IP_ADDRESS) //or, IP_ADDRESS, UPTIME, NONE
                    ->get_app();

#define SERIAL_DEBUG_DISABLED = True  // Also uncomment debug lines in parser and helper
#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  // Software serial port is used for receiving data on ESP8266
  // ESP8266 pins are specified as DX
  // ESP32 pins are specified as just the X in GPIOX

#ifdef ESP8266
  uint8_t pin = D4;
  SoftwareSerial* serial = new SoftwareSerial(pin, -1, false);
  serial->begin(19200, SWSERIAL_8N1);
#elif defined(ESP32)

//  Assign pins for ESP32 serial communication.
//  Uncomment the ones you plan to use.
//  The first 3 are connected to hardware UARTs
//  The next 3 are software UARTs.
//  Unknown if more could be defined.
//
//  Define hardware UARTs
//  Use these first if you have up to two ve.direct MPPTs
//  These are labeled 1, 2, 3 in the Signal K path names

  uint8_t pinA = 27;   // Reassign UART1 to pin 27
  HardwareSerial* serialA = &Serial1;
  serialA->begin(19200, SERIAL_8N1, pinA, -1);

  // uint8_t pinB = 16;   //  Keep UART2 on default pin 16
  // HardwareSerial* serialB = &Serial2;
  // serialB->begin(19200, SERIAL_8N1, pinB, -1);

//  Note that UART0 defaults to the USB chip on many dev kits
//  Seems to work fine though you must reassign the pin.
  // uint8_t pinC = 26;  // Reassign UART0 to pin 26
  // HardwareSerial* serialC = &Serial;
  // serialC->begin(19200, SERIAL_8N1, pinC, -1);
 
//  Define the rest of the serial ports using software serial
//  These are labeled 4, 5, 6 in the Signal K path names

  // uint8_t pinD = 17;  // Use software serial on pin 17
  // SoftwareSerial* serialD = new SoftwareSerial(pinD, -1, false);
  // serialD->begin(19200, SWSERIAL_8N1);

  // uint8_t pinE = 25;  // Use software serial on pin 25
  // SoftwareSerial* serialE = new SoftwareSerial(pinE, -1, false);
  // serialE->begin(19200, SWSERIAL_8N1);

  // uint8_t pinF = 33;  // Use software serial on pin 33
  // SoftwareSerial* serialF = new SoftwareSerial(pinF, -1, false);
  // serialF->begin(19200, SWSERIAL_8N1);

  // Enable level shifter
  // Wire to OE pin on 8 channel single chip TXS0108E
  uint8_t pinLevelShifterPower = 23;
  pinMode(pinLevelShifterPower, OUTPUT);
  digitalWrite(pinLevelShifterPower, HIGH);


#endif

// Instantiate each MPPT charger
// auto* charger1 = setup_vedirect(serialA, 1);
// The serial[A..F] refers to the serial ports/pins
// The number is concatenated to the Signal K path.
// The variables are not used elsewhere, so you will
// likely get a warning.  


  auto* charger1 = setup_vedirect(serialA, 1);
  // String("Davits")->(
  // new SKOutputString("electrical.solar.charger1.name", "/ve.direct.1/chargerNamePath"));
  //    Marco - this^^ is where I was trying to hard code a name
  //    so it could be displayed etc.

  // auto* charger2 = setup_vedirect(serialB, 2);

  // auto* charger3 = setup_vedirect(serialC, 3);

  // auto* charger4 = setup_vedirect(serialD, 4);

  // auto* charger5 = setup_vedirect(serialE, 5);

  // auto* charger6 = setup_vedirect(serialF, 6);

  sensesp_app->enable();
});
