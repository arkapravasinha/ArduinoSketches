#include "M5Stack.h"
void setup() {
  M5.begin(); //Init M5Stack(Initial I2C is also included).  初始化M5Stack(初始化I2C也包含在内)
  M5.Power.begin();
  pinMode(36,INPUT);
  Serial.begin(9600); //initialise serial monitor

}

void loop() {
  int temp=analogRead(36);      //assign value of LDR sensor to a temporary variable
  Serial.println("Intensity="); //print on serial monitor using ""
  Serial.println(temp);         //display output on serial monitor
  delay(300);
}
