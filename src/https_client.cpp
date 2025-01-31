#include "https_client.h"
#include "config.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

void httpsClient(const char* host, const char* uri) {
    if (WiFi.status() != WL_CONNECTED) return;

    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setInsecure();

    HTTPClient https;
    Serial.print("[HTTPS] begin...\n");

    if (https.begin(*client, host, 443, uri)) {
        Serial.printf("[HTTPS] GET to %s\n", host);
        int httpCode = https.GET();

        if (httpCode > 0) {
            Serial.printf("[HTTPS] Response code: %d\n", httpCode);
            if (httpCode == HTTP_CODE_OK) {
                String payload = https.getString();
                Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTPS] Error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
    } else {
        Serial.println("[HTTPS] Unable to connect");
    }
}
