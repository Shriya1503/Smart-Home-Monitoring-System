#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// CE and CSN pins
RF24 radio(8, 7);

void setup(void) {
  while (!Serial);
  Serial.begin(9600);
  
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x76);
  
  // Open pipes for writing and reading
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  const uint64_t pipe = 0xE8E8F0F0E1LL;
  radio.openReadingPipe(1, pipe);
  
  radio.enableDynamicPayloads();
  radio.powerUp();
  dht.begin(); // Initialize sensor
}

void loop(void) {
  radio.startListening();
  Serial.println("Starting loop. Radio on");
  
  char receivedMessage[32] = {0};
  
  if (radio.available()) {
    radio.read(receivedMessage, sizeof(receivedMessage));
    Serial.println(receivedMessage);
    Serial.println("Turning off the radio");
    radio.stopListening();
    
    String stringMessage(receivedMessage);
    
    if (stringMessage == "GETSTEMP") {
      Serial.println("Looks like they want Temperature and Humidity data");
      
      char Humi[7];
      float humi = dht.readHumidity();
      dtostrf(humi, 3, 2, Humi);
      
      char Temp[5];
      float tempC = dht.readTemperature();
      dtostrf(tempC, 2, 2, Temp);
      
      char text[32];
      char hum[] = "Humidity= ";
      char T[] = "% Temp= ";
      
      String combinedStr;
      combinedStr = hum;
      combinedStr += Humi;
      combinedStr += T;
      combinedStr += Temp;
      combinedStr.toCharArray(text, sizeof(text));
      
      radio.write(text, sizeof(text));
      
      Serial.print("\nWe sent our message, Humidity=");
      Serial.print(humi);
      Serial.print("% | Temperature= ");
      Serial.print(tempC);
      Serial.println("C");
    } else {
      char msg[] = "No matching command received";
      radio.write(msg, sizeof(msg));
    }
  }
  delay(1000);
}