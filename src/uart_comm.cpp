#include "uart_comm.h"
#include <Arduino.h>

char tx_data[100];
STM32_STATE stm_state = STM32_IDLE;
bool ESP8266_SENT_SIZE = false;

void uart_transmit_size(Weather_data_t* data) {
    size_t tx_data_size = 0;

    snprintf(tx_data, sizeof(tx_data), "%d,%d,%d,%d,%d,%d,%d,%d,%d",
    data->id,
    data->temp,
    data->feels_like,
    data->pressure,
    data->humidity,
    data->visibility,
    data->wind_speed,
    data->wind_deg,
    data->clouds);
    
    // check for null character
    for (size_t i = 0; tx_data[i] > 0; i++)
    {
        tx_data_size++;
    }

    // make sure that size info is 3 bytes long
    if (tx_data_size < 10U) {
        Serial1.printf("00%d", tx_data_size);
    } else if (tx_data_size < 100U) {
        Serial1.printf("0%d", tx_data_size);
    } else if (tx_data_size < 1000U) {
        Serial1.print(tx_data_size);
    } else {
        ESP8266_SENT_SIZE = false;
        return;
    }

    ESP8266_SENT_SIZE = true;
    stm_state = STM32_IDLE;
}

void uart_transmit_data() {
    Serial1.print(tx_data);
    ESP8266_SENT_SIZE = false;
    stm_state = STM32_IDLE;
}