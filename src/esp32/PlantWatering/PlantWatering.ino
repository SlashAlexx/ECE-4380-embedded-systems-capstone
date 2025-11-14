// ESP32 project for Automatic Plant Watering WebSever
// Alex Roberts

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include "include/FileSystem.hpp"
#include "include/network.hpp"

AsyncWebServer webserver(80);
AsyncWebSocket ws("/ws");

void initWebSocket(){
  ws.onEvent(onEvent);
  webserver.addHandler(&ws);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len){
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT){
    data[len] = 0;
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
 void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT: {
        String latestDataString = getLatestFromJSON();
        client->text(latestDataString); // send data immediately on connect
        break;
    }
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  initializeFilesystem();
  
  bool wifi_status = beginWiFiConnection();
  if (!wifi_status){
    Serial.println("ERROR: Unable to connect to a known network...");
    return;
  }

  Serial.println(WiFi.localIP());
  initWebSocket();

  webserver.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", "text/html");
  });
  
  // Serve static files
  webserver.serveStatic("/", SPIFFS, "/");

  webserver.begin();

  String latestDataString = getLatestFromJSON();
}

void loop() {

 
  ws.cleanupClients();
}
