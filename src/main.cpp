
#include <WiFi.h>
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"

const char* ssid = "Conference2g";
const char* password = "MeeT@06#2";
const char* hostname = "esp32";

AsyncWebServer server(80);

String message = "Hello from your ESP32 web server! Server NO : 0";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin(hostname)) {
    Serial.println("Error setting up mDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", message);
  });

  server.begin();
  Serial.println("Server started");
}

void loop() {
  // ... (optional: handle other functionalities)
}

