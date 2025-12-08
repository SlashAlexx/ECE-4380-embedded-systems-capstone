// ESP32 project for Automatic Plant Watering WebSever
// Alex Roberts

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#include "include/FileSystem.hpp"
#include "include/network.hpp"

#define UART2_TX 17
#define UART2_RX 16

AsyncWebServer webserver(80);
AsyncWebSocket ws("/ws");

String receivedUART = "";

void initWebSocket(){
  ws.onEvent(onEvent);
  webserver.addHandler(&ws);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len){
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->opcode == WS_TEXT){
    data[len] = 0;
    String msg = (char*)data;

    Serial.print("Printing to UART: ");
    Serial.println(msg);

    Serial2.print(msg);
    Serial2.print("\n");
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

void processUART(String jsonStr){
  StaticJsonDocument<1024> doc;
  DeserializationError err = deserializeJson(doc, jsonStr);

  if (!err) {
      String wsPayload;
      serializeJson(doc, wsPayload);
      ws.textAll(wsPayload); // send to all connected clients
  } else {
      Serial.println("JSON parse error");
  }

}

void setup() {
  Serial.begin(115200); // Serial Debugging
  Serial2.begin(115200, SERIAL_8N1, UART2_RX, UART2_TX); // STM32 UART Connection

  // --- Confirm STM32 UART Connection ---
  Serial2.println("ACK");

  String UART_ACK = "";
  unsigned long t0 = millis();
  while (millis() - t0 < 2000) {  // 2-second timeout
      if (Serial2.available()) {
          char c = Serial2.read();
          if (c == '\n') {
              UART_ACK = receivedUART;
              break;
          } else {
              receivedUART += c;
          }
      }
  }

  receivedUART = "";
  UART_ACK.trim();
  if (UART_ACK == "ACK") {
      Serial.println("STM32 UART Connection Established");
  } else {
      Serial.println("Unable to establish UART Connection with STM32");
      //while (1);
  }

  // -- Init WiFi Connection ---
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
}

void loop() {

  // Continuously check for incoming UART data
  while (Serial2.available()){
    char c = Serial2.read();
    if (c == '\n'){
      processUART(receivedUART);
      receivedUART = ""; // Clear
    }
    else receivedUART += c;
  }

  ws.cleanupClients();
}
