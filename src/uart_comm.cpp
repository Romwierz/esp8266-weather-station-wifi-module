#include "uart_comm.h"
#include <Arduino.h>

void uart_transmit(Weather_data_t* data) {
    char tx_data[100];
    size_t tx_data_size = 0;

    snprintf(tx_data, 100, "%d,%d,%d,%d,%d",
    data->temp,
    data->feels_like,
    data->humidity,
    data->pressure,
    data->wind_speed);
    
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
        // not wanted
    }

    delay(20);

    Serial1.print(tx_data);
}