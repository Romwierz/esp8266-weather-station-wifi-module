#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <ArduinoJson.h>

typedef struct {
    int id;
    int temp;
    int feels_like;
    int pressure;
    int humidity;
    int visibility;
    int wind_speed;
    int wind_deg;
    int clouds;
} Weather_data_t;

extern Weather_data_t parsedData;

void parseWeatherData(const String &payload);

#endif