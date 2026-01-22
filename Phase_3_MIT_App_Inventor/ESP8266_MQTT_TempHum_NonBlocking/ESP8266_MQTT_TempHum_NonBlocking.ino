#include <Adafruit_Sensor.h> 
#include <DHT_U.h> 
#include <ESP8266WiFi.h> 
#include <PubSubClient.h> 
#include <ArduinoJson.h> 

#define DHTPIN D1
#define LED D2
#define Fan D3
#define DHTTYPE DHT22

DHT_Unified dht(DHTPIN, DHTTYPE);

const char* ssid = "LAPTOP-78RKKJ2E 0340";
const char* password = "12345678";
const char* mqttServer = "192.168.137.62"; 
const char* mqttUserName = "homesystem";  
const char* mqttPwd = "Shriya";  
const char* clientID = "temphumi"; 
const char* topic = "TempHum"; 

unsigned long previousMillis = 0;
const long interval = 5000;
String msgStr = "";      
float temp, hum;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("MQTT connected");
      client.subscribe("lights");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void callback(char*topic, byte* payload, unsigned int length) {
  String data = "";
  for (int i = 0; i < length; i++) {
    data += (char)payload[i];
  }
  if(data=="ON"){
    digitalWrite(LED, HIGH);
  } else if(data=="OFF"){
    digitalWrite(LED, LOW);
  } else {
    int fanspeed=data.toInt();
    analogWrite(Fan,fanspeed);
  }
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(Fan,OUTPUT);
  analogWrite(Fan,0);
  setup_wifi();
  client.setServer(mqttServer, 1883); 
  client.setCallback(callback); 
}

void loop() {
  if (!client.connected()) { 
    reconnect(); 
  }
  client.loop();
  
  unsigned long currentMillis = millis(); 
  if (currentMillis - previousMillis >= interval) { 
    previousMillis = currentMillis;
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (!isnan(event.temperature)) {
      temp = event.temperature;
    }
    dht.humidity().getEvent(&event);
    if (!isnan(event.relative_humidity)) {
      hum = event.relative_humidity;
    }
    
    msgStr = String(temp)+","+String(hum);
    byte arrSize = msgStr.length() + 1;
    char msg[arrSize];
    msgStr.toCharArray(msg, arrSize);
    client.publish(topic, msg);
    msgStr = "";
  }
}