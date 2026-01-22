#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <MFRC522.h>

#define SS_PIN D2  // SDA connected to D1
#define RST_PIN D1 // RST connected to D2
MFRC522 rfid(SS_PIN, RST_PIN);

#define GREEN_LED D3 
#define RED_LED D4  

const char* ssid = "LAPTOP-78RKKJ2E 0340";
const char* password = "12345678";
const char* MQTT_username = "homesystem";
const char* MQTT_password = "Shriya";
const char* mqtt_server = "192.168.0.0"; //you ip address

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", MQTT_username, MQTT_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

bool isAuthorized() {
  byte knownTag1[] = {0xEE, 0x90, 0x4F, 0x1D}; // Replace with your authentic tag's UID
  byte knownTag2[] = {0xAB, 0xCD, 0xEF, 0x01}; 
  
  byte* currentTag = rfid.uid.uidByte;
  int tagSize = rfid.uid.size;
  
  if (tagSize == sizeof(knownTag1) && memcmp(currentTag, knownTag1, tagSize) == 0) {
    return true; 
  } else if (tagSize == sizeof(knownTag2) && memcmp(currentTag, knownTag2, tagSize) == 0) {
    return true; 
  }
  return false; 
}

void readRFID() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;
  
  String cardData = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    cardData += String(rfid.uid.uidByte[i], HEX);
  }
  Serial.print("RFID Card: ");
  Serial.println(cardData);
  
  if (isAuthorized()) {
    Serial.println("Access granted!");
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED,LOW);
    String message = "Access granted for RFID: " + cardData;
    client.publish("RFID/Entry",message.c_str());
  } else {
    Serial.println("Access denied!");
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    String message = "Access Denied for RFID: " + cardData;
    client.publish("RFID/Entry",message.c_str());
  }
  delay(1000);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop()) { 
    Serial.println("MQTT connection retrying");
    client.connect("ESP8266Client", MQTT_username, MQTT_password);
  }
  readRFID();
}