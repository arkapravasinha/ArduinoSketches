#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include <M5Stack.h>
#include "DHT.h"

#define DHTPIN 5 
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "jarvis";
const char* password = "arka@subarna";

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.0.105";
int        port     = 1883;
const char topic[]  = "m5stack/dht22/temp";
const char topic2[]  = "m5stack/dht22/humidity";
const char topic3[]  = "m5stack/dht22/heatindex";
const char topic4[]  ="m5stack/dht22/commands";

//set interval for sending messages (milliseconds)
const long interval = 5000;
unsigned long previousMillis = 0;

int count = 0;

int command=0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(10);

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(2000);
  }

  Serial.println("You're connected to the network");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  mqttClient.onMessage(onMqttMessage);
  mqttClient.subscribe(topic4);
  dht.begin();
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval && command==0) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println(t);

    Serial.print("Sending message to topic: ");
    Serial.println(topic2);
    Serial.println(h);

    Serial.print("Sending message to topic: ");
    Serial.println(topic2);
    Serial.println(hic);

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic);
    mqttClient.print(t);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic2);
    mqttClient.print(h);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic3);
    mqttClient.print(hic);
    mqttClient.endMessage();

    Serial.println();
  }
}

void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.println("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");
  
    char inChar;

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    inChar = (char)mqttClient.read();  //get next character
    switch (inChar)
    {
      case '0': //if it is a 0
        Serial.println("Off");
        command=1;
        break;
      case '1': //if it is a 1
        Serial.println("On");
        command=0;
        break;
      default:  //otherwise
        Serial.println("got something else");
    }
  }
  Serial.println();
  Serial.println();
}
