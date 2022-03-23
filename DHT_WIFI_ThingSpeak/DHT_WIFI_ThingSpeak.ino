#include <WiFi.h>
#include <M5Stack.h>
#include "DHT.h"
#include "ThingSpeak.h"

#define DHTPIN 5 
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "jarvis";
const char* password = "arka@subarna";

unsigned long myChannelNumber = 1683132;
const char * myWriteAPIKey = "M4WUTXOUG9DILIDV";
String myStatus = "";
WiFiClient wifiClient;


//set interval for sending messages (milliseconds)
const long interval = 20000;
unsigned long previousMillis = 0;

int count = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(10);

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);   
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(2000);
  }

  Serial.println("You're connected to the network");
  Serial.println(WiFi.localIP());
  Serial.println();
  ThingSpeak.begin(wifiClient);
  dht.begin();
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Sending Temp to thingspeak: ");
    Serial.println(t);

    Serial.print("Sending Humidity to thingspeak: ");
    Serial.println(h);

    Serial.print("Sending Heat Index to thingspeak: ");
    Serial.println(hic);
    ThingSpeak.setField(1, t);
    ThingSpeak.setField(2, h);
    ThingSpeak.setField(3, hic);
    myStatus = String("Value extracted successfully"); 
    
    ThingSpeak.setStatus(myStatus);
  
    // write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }

    Serial.println();
  }
}
