// Implementation for Filesystem Related Functions
// Alex Roberts

#include "include/FileSystem.hpp"

#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>


bool initializeFilesystem(){
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return false;
    }
    return true;
}

// Helper function to return JSON in string format
String getLatestFromJSON(){

    File latest_file = SPIFFS.open("/watering-data.json", "r");
    if (!latest_file){
        Serial.println("Unable to locate latest data");
        return "{}";
    }

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, latest_file);
    latest_file.close();
    
    if (error) {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.f_str());
        return "{}";
    }
    
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

uint8_t getKnownNetworkCount(){
  
    if (!SPIFFS.exists("/auth.json")) {
        Serial.println("Unable to locate Auth File");
        return 0;
    }

    File file = SPIFFS.open("/auth.json", "r");
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    if (error) {
        Serial.println("Failed to parse auth JSON");
        return 0;
    }
    
    return doc["network_count"].as<uint8_t>();
}

authCredentials getNetworkAuth(uint8_t network_index){

  authCredentials auth; // Empty Default
  File file = SPIFFS.open("/auth.json", "r");
  if (!file) return auth;
    
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) return auth;

  String networkKey = "network" + String(network_index);
  auth.ssid = String(doc[networkKey]["ssid"].as<const char*>());
  auth.pswd = String(doc[networkKey]["password"].as<const char*>());
  return auth;
}
