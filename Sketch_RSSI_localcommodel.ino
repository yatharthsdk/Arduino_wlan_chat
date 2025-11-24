#include <WiFiS3.h>


char ssid[] = "Spider";
char pass[] = "YOUR_PASSWORD"; 

int status = WL_IDLE_STATUS;
const int serverPort = 5050;
WiFiServer server(serverPort);

// RSSI toggle flag
bool rssiEnabled = false;

void setup() {
  Serial.begin(9600);
  while (!Serial) {;}

  Serial.println("=== UNO R4 WiFi - Chat Server (with RSSI toggle) ===");

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("[ERROR] No WiFi module.");
    while (true) { delay(1000); }
  }

  // Connect to WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  Serial.println("[OK] Connected to WiFi!");
  Serial.print("Arduino IP: ");
  Serial.println(WiFi.localIP());

  Serial.print("Starting server on port ");
  Serial.println(serverPort);
  server.begin();
}

void handleCommandFromClient(const String &cmd, WiFiClient &client) {
  if (cmd == "RSSI_ON") {
    rssiEnabled = true;
    client.println("[Arduino] RSSI reporting ENABLED");
    Serial.println("[CMD] RSSI_ON -> enabled");
  } else if (cmd == "RSSI_OFF") {
    rssiEnabled = false;
    client.println("[Arduino] RSSI reporting DISABLED");
    Serial.println("[CMD] RSSI_OFF -> disabled");
  } else {
    // normal text message
    Serial.print("From client: ");
    Serial.println(cmd);
    client.print("Arduino received: ");
    client.println(cmd);
  }
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client connected!");

    client.println("Hello from Arduino UNO R4 WiFi server!");
    client.println("Type RSSI_ON or RSSI_OFF to control signal reporting.");

    String buffer = "";
    unsigned long lastRssiTime = 0;

    while (client.connected()) {
      // 1) Handle data from client 
      while (client.available()) {
        char c = client.read();
        if (c == '\n') {
          buffer.trim();
          if (buffer.length() > 0) {
            handleCommandFromClient(buffer, client);
          }
          buffer = "";
        } else {
          buffer += c;
        }
      }

      // 2) Send text from Serial Monitor to client
      if (Serial.available()) {
        String msg = Serial.readStringUntil('\n');
        msg.trim();
        if (msg.length() > 0) {
          client.println(msg);
          Serial.print("You -> client: ");
          Serial.println(msg);
        }
      }

      // 3) Periodic RSSI only if enabled
      if (rssiEnabled && millis() - lastRssiTime > 5000) {
        lastRssiTime = millis();
        long rssi = WiFi.RSSI();
        String rssiMsg = "RSSI: " + String(rssi) + " dBm";
        client.println(rssiMsg);
        Serial.println(rssiMsg);
      }
    }

    Serial.println("Client disconnected.");
    client.stop();
  }
}