#include "config.h"
#include "network_manager.h"
#include "https_client.h"
#include "uart_comm.h"

#ifdef WITH_GDB
#include <GDBStub.h>
#endif

#include "certs.h"

unsigned long ledBlinkPreviousTime;
const unsigned long ledBlinkInterval = 1000;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(FLASH_BTN, INPUT);
  Serial.begin(115200);
  delay(10);
  Serial1.begin(115200);
  delay(10);

  #ifdef WITH_GDB
  gdbstub_init();
  gdb_do_break();
  #endif

  readConfig();
  setupNetwork(ssid, password);

  snprintf(api_call_uri, sizeof(api_call_uri), "/data/2.5/weather?q=Warsaw&lang=pl&appid=%s&units=metric", api_key);

  ledBlinkPreviousTime = millis();
}

void loop() {
  updateNetwork();
  
  unsigned long ledBlinkDiff = millis() - ledBlinkPreviousTime;
  if(ledBlinkDiff > ledBlinkInterval)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    ledBlinkPreviousTime += ledBlinkDiff;
  }

  if (BTN_PRESSED(FLASH_BTN))
  {
    showIPAddress();
    Serial.println("Downloading weather data...");
    test_uart_transmit();
    delay(200);
  }
}
