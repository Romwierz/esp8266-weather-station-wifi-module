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

    parsedData.id = (int) doc["weather"][0]["id"];
    parsedData.temp = (int) doc["main"]["temp"];
    parsedData.feels_like = (int) doc["main"]["feels_like"];
    parsedData.pressure = doc["main"]["pressure"];
    parsedData.humidity = doc["main"]["humidity"];
    parsedData.visibility = doc["visibility"];
    parsedData.wind_speed = (int) doc["wind"]["speed"];
    parsedData.wind_deg = (int) doc["wind"]["deg"];
    parsedData.clouds = (int) doc["clouds"]["all"];

    Serial.printf("ID: %d, Temp: %d°C, Feels like: %d°C, Pressure: %d hPa, Humidity: %d%%, Visibility: %d\n",
                  parsedData.id, parsedData.temp, parsedData.feels_like,
                  parsedData.pressure, parsedData.humidity, parsedData.visibility);
}