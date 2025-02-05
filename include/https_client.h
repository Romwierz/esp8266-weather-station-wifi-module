#ifndef HTTPS_CLIENT_H
#define HTTPS_CLIENT_H

#include <ESP8266HTTPClient.h>

extern String httpResponsePayload;

void httpsClient(const char* host, const char* uri);

#endif