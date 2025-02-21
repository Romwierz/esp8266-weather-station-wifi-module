#include "config.h"
#include "network_manager.h"
#include "https_client.h"
#include "uart_comm.h"

#ifdef WITH_GDB
#include <GDBStub.h>
#endif

#include "certs.h"

bool ESP8266_SENT_SIZE = false;
int data_selector = 0;

Weather_data_t testParsedData = {
  .temp = 1,
  .feels_like = 2,
  .humidity = 3,
  .pressure = 4,
  .wind_speed = 5005
};

Weather_data_t testParsedData2 = {
  .temp = 1,
  .feels_like = 2,
  .humidity = 3,
  .pressure = 4,
  .wind_speed = 5
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
  }
}