###############################################################
#   Arduino UNO R4 WiFi 📡 — Local Chat + RSSI Toggle System  #
###############################################################



This project implements a **local network chat** and **WiFi signal monitor** using:

- **Arduino UNO R4 WiFi** as a TCP server  
- A **Python client** running on a laptop (macOS in my case)  

It demonstrates end-to-end communication between an embedded device and a host over WiFi using simple TCP sockets.

---

## Features

- Arduino acts as a **WiFi TCP server**.
- Python script connects as a **TCP client**.
- **Two-way text chat**:
  - Type in the Python terminal → message appears in Arduino Serial Monitor.
  - Type in Arduino Serial Monitor → message appears in Python terminal.
- **Toggleable RSSI reporting**:
  - `RSSI_ON` → Arduino periodically sends WiFi signal strength.
  - `RSSI_OFF` → stops RSSI messages.
- Good example of **embedded networking + systems debugging**.

---

## Hardware & Software

**Hardware:**
- Arduino UNO R4 WiFi
- USB cable

**Software:**
- Arduino IDE (with `Arduino UNO R4 Boards` + `WiFiS3` support)
- Python 3.x
- Laptop on the **same WiFi network** as the Arduino…

---

## Arduino Sketch (Server)

The Arduino:

- Connects to WiFi
- Starts a `WiFiServer` on a given port (e.g. `5050`)
- Waits for a client to connect
- Handles:
  - Commands from client (`RSSI_ON`, `RSSI_OFF`)
  - Normal text messages from client
  - Text messages coming from Serial Monitor
  - Optional RSSI reporting

Key points in the sketch:

```cpp
#include <WiFiS3.h>

char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

int status = WL_IDLE_STATUS;
const int serverPort = 5050;
WiFiServer server(serverPort);
bool rssiEnabled = false;

// ... setup(): connect to WiFi, print Arduino IP, start server
// ... loop(): accept client, process text, handle RSSI_ON/OFF, send RSSI when enabled
