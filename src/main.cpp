#include "config.h"
#include "network_manager.h"
#include "https_client.h"
#include "uart_comm.h"

#ifdef WITH_GDB
#include <GDBStub.h>
#endif

#include "certs.h"

int data_selector = 0;

Weather_data_t testParsedData = {
  .id = 800,
  .temp = 11,
  .feels_like = 13,
  .pressure = 999,
  .humidity = 55,
  .visibility = 10000,
  .wind_speed = 121,
  .wind_deg = 280,
  .clouds = 75
};

Weather_data_t testParsedData2 = {
  .id = 500,
  .temp = -2,
  .feels_like = -3,
  .pressure = 1012,
  .humidity = 71,
  .visibility = 8000,
  .wind_speed = 89,
  .wind_deg = 95,
  .clouds = 30
};

Weather_data_t data = testParsedData;

IRAM_ATTR void dataRequestCallback() {
  Serial.println("Request detected!");
  Serial.println(stm_state);

  if ((stm_state == STM32_IDLE) && (digitalRead(INT_PIN) == 0)) {
    stm_state = STM32_REQ_SIZE;
  }
  else if ((stm_state == STM32_IDLE) && (digitalRead(INT_PIN) == 1) && (ESP8266_SENT_SIZE == true)) {
    stm_state = STM32_REQ_DATA;
  }
  else {
    stm_state = STM32_IDLE;
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(FLASH_BTN, INPUT);
  pinMode(INT_PIN, INPUT);
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

  attachInterrupt(digitalPinToInterrupt(INT_PIN), dataRequestCallback, CHANGE);
}

void loop() {
  updateNetwork();

  if (stm_state == STM32_REQ_SIZE) {
    showIPAddress();
    Serial.println("Downloading weather data...");
#ifndef TEST_DATA
    httpsClient(API_OPEN_WEATHER_MAP, api_call_uri);
    parseWeatherData(httpResponsePayload);
    uart_transmit_size(&parsedData);
#else
    if (data_selector == 0) {
      uart_transmit_size(&testParsedData);
      data_selector = 1;
    }
    else {
      uart_transmit_size(&testParsedData2);
      data_selector = 0;
    }
#endif  
  } else if (stm_state == STM32_REQ_DATA) {
    Serial.println("Sending weather data...\n");
    uart_transmit_data();
    ESP.deepSleep(0);
  }
}