#include "config.h"

char ssid[32];
char password[32];
char api_key[40];
char api_call_uri[100];

void readConfig() {
    while (!SPIFFS.begin()){
        Serial.println("Failed to mount file system");
    }

    File file = SPIFFS.open("/config.txt", "r");
    if (!file) {
        Serial.println("Failed to open config file");
        return;
    }

    String line;
    while (file.available()) {
        line = file.readStringUntil('\n');
        line.trim();

        if (line.startsWith("ssid:")) {
            line.remove(0, 5);
            line.toCharArray(ssid, sizeof(ssid));
        } else if (line.startsWith("password:")) {
            line.remove(0, 9);
            line.toCharArray(password, sizeof(password));
        } else if (line.startsWith("api_key:")) {
            line.remove(0, 8);
            line.toCharArray(api_key, sizeof(api_key));
        }
    }

    file.close();
}
