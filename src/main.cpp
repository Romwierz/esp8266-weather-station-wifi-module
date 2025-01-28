#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#ifdef WITH_GDB
#include <GDBStub.h>
#endif

#include "certs.h"

#ifndef NODEMCU
// for ESP-01S it has to be changed to GPIO2
#define LED_BUILTIN 2
#endif

char ssid[32];
char password[32];

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
  }

  file.close();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
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
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);

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
}

void loop() {
  MDNS.update();
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  Serial.println("ESP8266 IP address: " + WiFi.localIP().toString());
  delay(500);
}
