#ifndef UART_COMM_H
#define UART_COMM_H

void uart_transmit(float temp, float feels_like, int humidity, int pressure, float wind_speed);
void test_uart_transmit();

#endif