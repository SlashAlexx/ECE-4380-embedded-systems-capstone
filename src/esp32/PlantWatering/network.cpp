// Header file for network related functionality
// Alexander Roberts

#include "network.hpp"
#include "FileSystem.hpp"

bool beginWiFiConnection(){

  uint8_t network_count = getKnownNetworkCount();
  for (int i = 0; i < network_count; i++){

    authCredentials *creds = getNetworkAuth(i);
    WiFi.begin(creds.ssid, creds.pswd);

    uint8_t connection_count = 0;
    Serial.println("Connecting to WiFi..");
    while (WiFi.status() != WL_CONNECTED){
      delay(500); // 0.5 seconds
      if (++connection_count >= CONNECTION_RETRIES){
        Serial.println("Unable to connect to network: " + creds.ssid);
        break; // Tries next available network
      }
    }
    Serial.println("Connected to network: " + creds.ssid);
    return true;
  }
  return false;
}