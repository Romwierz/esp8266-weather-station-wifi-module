#include "config.h"
#include "network_manager.h"
#include "https_client.h"
#include "uart_comm.h"

#ifdef WITH_GDB
#include <GDBStub.h>
#endif

#include "certs.h"

bool ESP8266_SENT_SIZE = false;

Weather_data_t testParsedData = {
  .temp = 1,
  .feels_like = 2,
  .humidity = 3,
  .pressure = 4,
  .wind_speed = 50
};

IRAM_ATTR void dataRequestCallback() {
  Serial.println("Request detected!");
  switch (stm_state)
  {
  case STM32_IDLE:
    stm_state = (ESP8266_SENT_SIZE == false) ? STM32_REQ_SIZE : STM32_REQ_DATA;
    break;
  default:
    stm_state = STM32_IDLE;
    break;
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(FLASH_BTN, INPUT);
  Serial.begin(115200);
  delay(10);
  Serial1.begin(115200);
  delay(10);

  attachInterrupt(digitalPinToInterrupt(FLASH_BTN), dataRequestCallback, FALLING);

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

  if (stm_state == STM32_REQ_SIZE) {
    showIPAddress();
    Serial.println("Downloading weather data...");
    httpsClient(API_OPEN_WEATHER_MAP, api_call_uri);
    parseWeatherData(httpResponsePayload);
    uart_transmit_size(&parsedData);
    ESP8266_SENT_SIZE = true;
  } else if (stm_state == STM32_REQ_DATA) {
    Serial.println("Sending weather data...");
    uart_transmit_data();
    ESP8266_SENT_SIZE = false;
  }
}
