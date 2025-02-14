#ifndef CONFIG_H
#define CONFIG_H

#include <FS.h>

#ifndef NODEMCU
// for ESP-01S it has to be changed to GPIO2
#define LED_BUILTIN 2
#endif
#define FLASH_BTN D3
#define INT_PIN D5
#define BTN_PRESSED(x) !digitalRead(x)

extern char ssid[32];
extern char password[32];
extern char api_key[40];
extern char api_call_uri[100];

void readConfig();

#endif
