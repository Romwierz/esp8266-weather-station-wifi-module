#include "json_parser.h"
#include "uart_comm.h"

Weather_data_t parsedData;

void parseWeatherData(const String &payload) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        Serial.print("JSON Parse Error: ");
        Serial.println(error.c_str());
        return;
    }

    parsedData.temp = (int) doc["main"]["temp"];
    parsedData.feels_like = (int) doc["main"]["feels_like"];
    parsedData.humidity = doc["main"]["humidity"];
    parsedData.pressure = doc["main"]["pressure"];
    parsedData.wind_speed = (int) doc["wind"]["speed"];

    Serial.printf("Temp: %d°C, Feels like: %d°C, Humidity: %d%%, Pressure: %d hPa\n",
                  parsedData.temp, parsedData.feels_like, parsedData.humidity, parsedData.pressure);
}