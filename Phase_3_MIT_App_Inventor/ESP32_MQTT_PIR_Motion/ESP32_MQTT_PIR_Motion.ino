#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_task_wdt.h>

#define timeSeconds 10

const char* ssid = "LAPTOP-78RKKJ2E 0340";
const char* password = "12345678";
const char* mqttServer = "192.168.137.62";
const int mqttPort = 1883;
const char* mqttTopic = "Movement";
const char* mqtt_username="homesystem";
const char* mqtt_password="Shriya";
const char* clientID="MovementData";

const int led = 27;
const int motionSensor = 26;

unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;
boolean motion = false;

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying...");
      delay(1000);
    }
  }
}

void connect_MQTT(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  
  client.setServer(mqttServer, 1883); // Ensure server is set before connecting
  
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  } else {
    Serial.println("Connection to MQTT Broker failed…");
  }
}

void IRAM_ATTR detectsMovement() {
  digitalWrite(led, HIGH);
  startTimer = true;
  lastTrigger = millis();
}

void setup() {
  Serial.begin(9600);
  pinMode(motionSensor, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
  connect_MQTT();
  
  Serial.setTimeout(2000);
  esp_task_wdt_init(10, true);  
}

void loop() {
  now = millis();
  if ((digitalRead(led) == HIGH) && (motion == false)) {
    Serial.println("MOTION DETECTED!!!");
    motion = true;
    client.publish(mqttTopic, "Access Granted"); // Sending alert
  }
  if (startTimer && (now - lastTrigger > (timeSeconds * 1000))) {
    Serial.println("Motion stopped...");
    digitalWrite(led, LOW);
    startTimer = false;
    motion = false;
  }
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}