
#include <WiFi.h>
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"
#include <FS.h>


const char* ssid = "Conference2g";
const char* password = "MeeT@06#2";
const char* hostname = "esp32";

String uploadPath = "/upload";

const String uploadFormHTML =
  "<html>"
  "<head><title>ESP32 App Upload</title></head>"
  "<body>"
  "<h1>Upload Your React App ZIP</h1>"
  "<form method=\"POST\" enctype=\"multipart/form-data\" action=\"" + uploadPath + "\">"
  "<input type=\"file\" name=\"uploadfile\" accept=\".zip\">"
  "<br><br>"
  "<button type=\"submit\">Upload</button>"
  "</form>"
  "</body>"
  "</html>";

AsyncWebServer server(80);

String message = "Hello from your ESP32 web server! Server NO : 0";

void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  String logmessage = "Client : " + request->client()->remoteIP().toString() + " " + request->url();
  Serial.println(logmessage);

  if (!index) {
    logmessage = "Upload Start: " + String(filename);
    // open the file on first call and store the file handle in the request object
    // request->_tempFile = SPIFFS.open("/" + filename, "w");
    Serial.println(logmessage);
  }

  if (len) {
    // stream the incoming chunk to the opened file
    // request->_tempFile.write(data, len);
    logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
    Serial.println(logmessage);
  }

  if (final) {
    logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
    // close the file handle as the upload is now done
    // request->_tempFile.close();
    Serial.println(logmessage);
    request->redirect("/");
  }
}


void configureWebServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Request for / received");
    request->send(200, "text/html", uploadFormHTML.c_str());
  });

  server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200);
  }, handleUpload);
}


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
  configureWebServer();
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // ... (optional: handle other functionalities)
}

