#include "json_parser.h"
#include "uart_comm.h"

void parseWeatherData(const String &payload) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        Serial.print("JSON Parse Error: ");
        Serial.println(error.c_str());
        return;
    }

    float temp = doc["main"]["temp"];
    float feels_like = doc["main"]["feels_like"];
    int humidity = doc["main"]["humidity"];
    int pressure = doc["main"]["pressure"];
    float wind_speed = doc["wind"]["speed"];

    Serial.printf("Temp: %.2f°C, Feels like: %.2f°C, Humidity: %d%%, Pressure: %d hPa\n",
                  temp, feels_like, humidity, pressure);
    uart_transmit(temp, feels_like, humidity, pressure, wind_speed);
}