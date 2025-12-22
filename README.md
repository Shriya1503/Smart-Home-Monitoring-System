# Smart Home Monitoring System

[cite_start]**Author:** Shriya Sinha (21BLC1395) [cite: 3, 4]  
[cite_start]**Organization:** UGC DAE Consortium for Scientific Research, Indore [cite: 14]  
[cite_start]**Department:** School of Electronics Engineering, VIT Chennai [cite: 11]

## 📖 Abstract
This project is a comprehensive Smart Home prototype developed to enhance home automation through the Internet of Things (IoT). [cite_start]The system integrates various smart devices to control lighting and fans, monitor vehicle movement, manage door security via RFID, and track temperature and humidity levels[cite: 44, 46]. 

The project was executed in three distinct phases, evolving from basic radio communication to a complex MQTT-based network with data visualization and mobile app control.

## ⚙️ Hardware & Software Requirements

[cite_start]**Hardware:** [cite: 108-127]
* Microcontrollers: Raspberry Pi 3B+, Arduino UNO, ESP-32, ESP-8266 (NodeMCU)
* Communication: NRF24L01 Transceiver (2.4GHz)
* Sensors: DHT22 (Temp/Hum), PIR Sensor (HC-SR501), RFID Reader (RC522)
* Actuators: 1 Channel Relay (5V), Solenoid Door Lock, LEDs, DC Fan
* Power: 3.7V LiPo Battery, 12V Adapter

[cite_start]**Software:** [cite: 101-107]
* **Languages:** Python (Raspberry Pi), C++ (Arduino IDE)
* **Protocols:** MQTT (Mosquitto Broker), SPI
* **Backend/Visualization:** Node-Red, InfluxDB, Grafana
* **Mobile Dev:** MIT App Inventor

## 🚀 Project Phases

### Phase 1: Point-to-Point Radio Communication
The initial phase established wireless communication between an Arduino UNO (Slave) and a Raspberry Pi (Master) using NRF24L01 modules. [cite_start]The Raspberry Pi requests temperature data, and the Arduino responds. [cite: 130, 147]

### Phase 2: MQTT with Data Visualization (Node-Red, InfluxDB, Grafana)
This phase introduced Wi-Fi-based communication using MQTT. 
* **Broker:** Mosquitto running on Raspberry Pi.
* **Node-Red:** Handles logic and data flow.
* **InfluxDB:** Stores time-series sensor data.
* [cite_start]**Grafana:** Visualizes historical temperature, humidity, and RFID access logs. [cite: 294-295]

### Phase 3: Mobile Application Control
A custom Android application was built using MIT App Inventor. This app communicates directly with the Raspberry Pi MQTT broker to:
* Toggle Lights and Fans.
* View Real-time Temperature/Humidity.
* Receive Door Lock status (RFID).
* [cite_start]Receive Motion Detection alerts. [cite: 704-706]

## 🔧 Setup & Installation

1.  **MQTT Broker:** Install Mosquitto on Raspberry Pi.
2.  **Dependencies:**
    * Arduino IDE: Install `PubSubClient`, `DHT sensor library`, `Adafruit Unified Sensor`, `MFRC522`.
    * Python (RPi): Install `spidev` and `RPi.GPIO`.
3.  **Credentials:** Update `ssid`, `password`, and `mqtt_server` IP in all Arduino/ESP sketches before uploading.

## 📄 License
[cite_start]Submitted in partial fulfillment for the award of the degree of Bachelor of Technology in Electronics and Computer Engineering at VIT Chennai. [cite: 6-9]
