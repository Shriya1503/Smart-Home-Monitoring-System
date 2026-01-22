#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h> 
#include <ArduinoJson.h> 

const char* ssid = "LAPTOP-78RKKJ2E 0340";
const char* password = "12345678";
const char* mqttServer = "192.168.0.0";   //your ip 
const int mqttPort = 1883; 
const char* mqttUserName = "homesystem"; 
const char* mqttPwd = "Shriya"; 
const char* clientID = "ESP32Client"; 

const char* lightsTopic = "lights";
const char* fanTopic = "Fan";

const int fanPin = D2; 
const int ledPin = D1; 

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  if (strcmp(topic, lightsTopic) == 0) {
    if (message == "ON") {
      digitalWrite(ledPin, HIGH);
    } else if (message == "OFF") {
      digitalWrite(ledPin, LOW);
    }
  } else if (strcmp(topic, fanTopic) == 0) {
    int fanSpeed = message.toInt();
    analogWrite(fanPin, fanSpeed);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("connected");
      client.subscribe(lightsTopic);
      client.subscribe(fanTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  pinMode(fanPin, OUTPUT);
  analogWrite(fanPin, 0);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}