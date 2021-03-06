/**
 * @file Simple_HCSR04.ino
 * @author Moritz Bergmann
 * @brief An example for unsing the Simple_HCSR04 library.
 * @version 1.0
 * @date 2020-12-18
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <Arduino.h>
#include <Simple_HCSR04.h>
#include <M5Stack.h>

int count=0;

const int ECHO_PIN = 5; /// the pin at which the sensor echo is connected
const int TRIG_PIN = 2; /// the pin at which the sensor trig is connected

Simple_HCSR04 *sensor;

void setup()
{
  M5.begin(); //Init M5Stack(Initial I2C is also included).  初始化M5Stack(初始化I2C也包含在内)
  M5.Power.begin();
  Serial.begin(9600);

  // create sensor object
  sensor = new Simple_HCSR04(ECHO_PIN, TRIG_PIN);
}

void loop()
{
  unsigned long distance = sensor->measure()->cm();
  Serial.print("distance: ");
  M5.Lcd.print("distance: ");
  Serial.print(distance);
  M5.Lcd.print(distance);
  Serial.print("cm\n");
  M5.Lcd.print("cm\n");
  delay(200);
  if(count==10)
  {
    
   M5.Lcd.setCursor(0,0);
   M5.Lcd.fillScreen(BLACK);
   count=0;
  }

  count++;
}
