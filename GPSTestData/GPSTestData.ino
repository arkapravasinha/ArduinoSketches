#include <SoftwareSerial.h>
#include <M5Stack.h>

// The serial connection to the GPS module
SoftwareSerial ss(17, 16);

void setup(){
  
  M5.begin();
  M5.Power.begin();
Serial.begin(57600);
ss.begin(9600);
}

void loop(){

if(ss.available() > 0)
{
// get the byte data from the GPS
byte gpsData = ss.read();
M5.Lcd.println(gpsData);
Serial.write(gpsData);
M5.Lcd.setCursor(0,0);
}

}
