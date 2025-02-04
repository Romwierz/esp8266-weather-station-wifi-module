#include "config.h"
#include "network_manager.h"
#include "https_client.h"
#include "uart_comm.h"

#ifdef WITH_GDB
#include <GDBStub.h>
#endif

#include "certs.h"

Weather_data_t testParsedData = {
  .temp = 1,
  .feels_like = 2,
  .humidity = 3,
  .pressure = 4,
  .wind_speed = 50
};

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
}

void loop() {
  updateNetwork();

  if (BTN_PRESSED(FLASH_BTN))
  {
    showIPAddress();
    Serial.println("Downloading weather data...");
    uart_transmit(&testParsedData);
    delay(200);
  }
}
