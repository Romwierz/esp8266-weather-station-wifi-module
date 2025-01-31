#include "uart_comm.h"
#include <Arduino.h>

void uart_transmit(float temp, float feels_like, int humidity, int pressure, float wind_speed) {
    String data = String(temp) + "," + String(feels_like) + "," + 
                  String(humidity) + "," + String(pressure) + "," + 
                  String(wind_speed) + "\n";
    Serial.print("Sent to STM32: ");
    Serial.println(data);
    Serial1.println(data);
}

void test_uart_transmit() {
    String testData = "1.81,-2.99,89,1025,5.66";
    Serial.print("Sent to STM32: ");
    Serial.println(testData);
    Serial1.println(testData);
}