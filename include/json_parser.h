#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <ArduinoJson.h>

typedef struct {
    int temp;
    int feels_like;
    int humidity;
    int pressure;
    int wind_speed;
} Weather_data_t;

extern Weather_data_t parsedData;

void parseWeatherData(const String &payload);

#endif