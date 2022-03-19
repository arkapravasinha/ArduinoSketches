#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include<M5Stack.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 17, TXPin = 16;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);
  ss.begin(GPSBaud);
  Serial.println();
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  M5.Lcd.setCursor(0,0);
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    M5.Lcd.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
  M5.Lcd.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    M5.Lcd.print(gps.location.lat(), 6);
    M5.Lcd.print(F(","));
    M5.Lcd.print(gps.location.lng(), 6);
  }
  else
  {
    M5.Lcd.print(F("INVALID"));
  }

  M5.Lcd.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    M5.Lcd.print(gps.date.month());
    M5.Lcd.print(F("/"));
    M5.Lcd.print(gps.date.day());
    M5.Lcd.print(F("/"));
    M5.Lcd.print(gps.date.year());
  }
  else
  {
    M5.Lcd.print(F("INVALID"));
  }

  M5.Lcd.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    M5.Lcd.print(gps.time.hour());
    M5.Lcd.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    M5.Lcd.print(gps.time.minute());
    M5.Lcd.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    M5.Lcd.print(gps.time.second());
    M5.Lcd.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    M5.Lcd.print(gps.time.centisecond());
  }
  else
  {
    M5.Lcd.print(F("INVALID"));
  }

  M5.Lcd.println();
}
