#include <Arduino.h>
#include <FS.h>

// for ESP-01S it has to be changed to GPIO2
#define LED_BUILTIN 2

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
}

void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  Serial.println("I am alive");
  delay(500);
}
