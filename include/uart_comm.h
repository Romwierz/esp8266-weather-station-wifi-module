#ifndef UART_COMM_H
#define UART_COMM_H

#include "json_parser.h"

enum STM32_STATE {
    STM32_IDLE,
    STM32_REQ_SIZE,
    STM32_REQ_DATA
};

extern STM32_STATE stm_state;
extern bool ESP8266_SENT_SIZE;

void uart_transmit_size(Weather_data_t* data);
void uart_transmit_data();

#endif