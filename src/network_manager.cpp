#include "network_manager.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Arduino.h>

void setupNetwork(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    
    Serial.println("\nConnected to WiFi");
    Serial.println("ESP8266 IP address: " + WiFi.localIP().toString());

    if (!MDNS.begin("esp-01s")) {
        Serial.println("Error setting up mDNS!");
    } else {
        Serial.println("mDNS responder started");
    }
}

void showIPAddress() {
    Serial.println("ESP8266 IP address: " + WiFi.localIP().toString());
}

void updateNetwork() {
    MDNS.update();
}