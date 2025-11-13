// Header file for File System Operations
// Alex Roberts

#include <Arduino.h>
#include <string.h>

typedef struct WateringTimeInstance{
    String occurance = "";
    String time = "";
    uint8_t amount = 0;
} WateringTimeInstance;

typedef struct WateringLogInstance{
    String date = "";
    String time = "";
    uint8_t amount = 0;
    bool isManual = false;
} WateringLogInstance;

typedef struct authCredentials{
    String ssid = "";
    String pswd = "";
} authCredentials;


bool initializeFilesystem();
String getLatestFromJSON();
uint8_t getKnownNetworkCount();
*authCredentials getNetworkAuth(uint8_t network_index); // Get's auth1 set from JSON
