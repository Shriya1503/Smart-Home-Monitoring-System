# MIT App Inventor Construction Guide

This guide details how to reconstruct the mobile application used in Phase 3 of the project.

## 📱 Extension Required
You must import the MQTT extension to enable communication with the Raspberry Pi broker.
* **Extension Name:** UrsPahoMqttClient
* **Action:** Import this .aix extension into your MIT App Inventor project palette.

## 🎨 Frontend Layout (Designer View)
Organize your screen components vertically. [cite_start]Make the screen "Scrollable"[cite: 711].

### 1. Connection Section
* **HorizontalArrangement**
    * **Button:** Rename to `ConnectButton`. Text: "Connect".
    * **Label:** Rename to `ConnectText`. Text: "Press to connect".

### [cite_start]2. Light & Fan Control [cite: 712]
* **HorizontalArrangement**
    * **Image:** Rename to `Bulb`. Upload images `BulbON.png` and `BulbOFF.png`.
    * **Label:** Rename to `BulbStatus`. Text: "ON/OFF".
* **VerticalArrangement** (Fan)
    * **Image:** Icon of a Fan.
    * **Slider:** Rename to `Slider1`. This controls fan speed.
    * **Label:** Rename to `FanValue`.

### [cite_start]3. Monitoring Section [cite: 712]
* **HorizontalArrangement**
    * **Image:** Thermometer Icon.
    * **Label:** Rename to `tempValue`. Text: "..."
    * **Image:** Humidity Icon.
    * **Label:** Rename to `HumiValue`. Text: "..."

### 4. Security Section
* **VerticalArrangement**
    * **Image:** Rename to `RFID`. Upload `DoorOpen.png` and `DoorClosed.png`.
    * **Label:** Rename to `RFIDText`.
    * **Image:** Rename to `PIR`. Upload `MovementDetected.png`.
    * **Label:** Rename to `Motion`.

---

## 🧩 Backend Logic (Blocks View)

Recreate these logical blocks in the Blocks Editor.

### 1. MQTT Connection Logic
**When `ConnectButton`.Click:**
* **If** `UrsPahoMqttClient1`.IsConnected:
    * Call `UrsPahoMqttClient1`.Disconnect
    * Set `ConnectText`.Text to "Disconnected"
    * Set `ConnectButton`.Text to "Connect"
* **Else:**
    * Call `UrsPahoMqttClient1`.Connect (CleanSession=True)
    * Set `ConnectText`.Text to "Click to disconnect"
    * Set `ConnectButton`.Text to "Disconnect"

### 2. Subscribe on Connect
**When `UrsPahoMqttClient1`.ConnectionStateChanged:**
* **If** `UrsPahoMqttClient1`.IsConnected:
    * Call Subscribe (`Topic="TempHum"`, QoS=0)
    * Call Subscribe (`Topic="RFID"`, QoS=0)
    * Call Subscribe (`Topic="Movement"`, QoS=0)

### 3. Control Logic (Publishing)
**When `Bulb`.Click:**
* **If** `Bulb`.Picture == `BulbOFF.png`:
    * Call Publish (`Topic="lights"`, `Message="ON"`)
    * Set `Bulb`.Picture to `BulbON.png`
* **Else:**
    * Call Publish (`Topic="lights"`, `Message="OFF"`)
    * Set `Bulb`.Picture to `BulbOFF.png`

**When `Slider1`.PositionChanged:**
* [cite_start]Call Publish (`Topic="Fan"`, `Message=get thumbPosition`) [cite: 766]
* *(Note: Ensure topic matches Arduino code "Fan", report images varied)*

### 4. Receiving Data (MessageReceived)
**When `UrsPahoMqttClient1`.MessageReceived:**
* **If** `Topic` == "TempHum":
    * *Parse CSV string (Temp,Humidity)*
    * Set `tempValue`.Text to Item 1
    * Set `HumiValue`.Text to Item 2
* **Else If** `Topic` == "RFID":
    * Set `RFIDText`.Text to `Message`
    * **If** `Message` contains "Access Granted":
        * Set `RFID`.Picture to `DoorOpen.png`
    * **Else:**
        * Set `RFID`.Picture to `DoorLock.png`
* **Else If** `Topic` == "Movement":
    * Set `PIR`.Picture to `MovementDetected.png`
    * Set `Motion`.Text to "Movement detected at now"
