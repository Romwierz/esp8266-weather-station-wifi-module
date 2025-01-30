#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#ifdef WITH_GDB
#include <GDBStub.h>
#endif

#include "certs.h"

#ifndef NODEMCU
// for ESP-01S it has to be changed to GPIO2
#define LED_BUILTIN 2
#endif
#define FLASH_BTN D3
#define BTN_PRESSED(x) !digitalRead(x)

char ssid[32];
char password[32];
char api_key[40];
char api_call_uri[100];

const char* headerKeys[] = {"date", "server"};
const size_t numberOfHeaders = 2;

unsigned long ledBlinkPreviousTime;
const unsigned long ledBlinkInterval = 1000;

void readConfig()
{
  File file = SPIFFS.open("/config.txt", "r");
  if (!file)
  {
    Serial.println("Failed to open config file");
    return;
  }

  String line;
  while (file.available())
  {
    line = file.readStringUntil('\n');
    line.trim();  // Remove any trailing whitespace

    if (line.startsWith("ssid:"))
    {
      line.remove(0, 5);  // Remove the "ssid:" part
      line.toCharArray(ssid, sizeof(ssid));
    }
    else if (line.startsWith("password:"))
    {
      line.remove(0, 9);  // Remove the "password:" part
      line.toCharArray(password, sizeof(password));
    }
    else if (line.startsWith("api_key:"))
    {
      line.remove(0, 8);  // Remove the "api_key:" part
      line.toCharArray(api_key, sizeof(api_key));
    }
  }

  file.close();
}

void httpsClient(const char* host, const char* uri)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

    if (strcmp(host, METEO_IMGW) == 0) {
      client->setFingerprint(fingerprint_imgw_pl);
    }
    else if (strcmp(host, W3) == 0) {
      client->setFingerprint(fingerprint_w3_org);
    }
    else if (strcmp(host, API_OPEN_WEATHER_MAP) == 0) {
      client->setFingerprint(fingerprint_open_weather_map_org);
    }

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, host, port, uri)) {
      
      Serial.print("[HTTPS] GET...\n");
      Serial.printf("[HTTPS] Host: %s\n", host);

      https.collectHeaders(headerKeys, numberOfHeaders);
      int httpCode = https.GET();

      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          Serial.println("[HTTPS] Headers:");
          for (int i = 0; i < https.headers(); i++) {
            String headerName = https.headerName(i);
            String headerValue = https.header(i);
            Serial.println(i);
            Serial.printf("%s: %s\n", headerName.c_str(), headerValue.c_str());
          }
          String payload = https.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.println("[HTTPS] Unable to connect");
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(FLASH_BTN, INPUT);
  Serial.begin(115200);
  delay(10);

  #ifdef WITH_GDB
  gdbstub_init();
  gdb_do_break();
  #endif

  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }

  readConfig();
  snprintf(api_call_uri, sizeof(api_call_uri), "/data/2.5/weather?q=Warsaw&appid=%s", api_key);
  Serial.print("SSID: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.println("ESP8266 IP address: " + WiFi.localIP().toString());

  if (!MDNS.begin("esp-01s"))
  { 
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");

  ledBlinkPreviousTime = millis();
}

void loop() {
  MDNS.update();
  
  unsigned long ledBlinkDiff = millis() - ledBlinkPreviousTime;
  if(ledBlinkDiff > ledBlinkInterval)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    ledBlinkPreviousTime += ledBlinkDiff;
  }

  if (BTN_PRESSED(FLASH_BTN))
  {
    Serial.println("ESP8266 IP address: " + WiFi.localIP().toString());
    httpsClient(API_OPEN_WEATHER_MAP, api_call_uri);
    delay(200);
  }
}
