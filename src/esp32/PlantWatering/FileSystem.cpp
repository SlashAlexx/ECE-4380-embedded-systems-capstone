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
    File latest_file = SPIFFS.open("/auth.json");
    if (!latest_file){
        Serial.println("Unable to locate Auth File");
        return "{}";
    }

    DynamicJsonDocument doc(1024);
    DeserializationError = deserializeJson(doc, latest_file);
    latest_file.close();

    if (error){
        Serial.print("Failed to parse JSON");
        Serial.println(error.f_str());
        return "{}";
    }
    return doc["network_count"];
}

*authCredentials getNetworkAuth(uint8_t network_index){

    File latest_file = SPIFFS.open("/auth.json");
    if (!latest_file){
        Serial.println("Unable to locate Auth File");
        return "{}";
    }

    DynamicJsonDocument doc(1024);
    DeserializationError = deserializeJson(doc, latest_file);
    latest_file.close();

    if (error){
        Serial.print("Failed to parse JSON");
        Serial.println(error.f_str());
        return "{}";
    }

    authCredentials auth;
    String network = "network" + network_index;
    auth.ssid = doc["network"]["ssid"];
    auth.pswd = doc["network"]["password"];

    return auth;
}