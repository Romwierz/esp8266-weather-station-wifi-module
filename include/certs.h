#pragma once

#define METEO_IMGW              "meteo.imgw.pl"
#define W3                      "jigsaw.w3.org"
#define API_OPEN_WEATHER_MAP    "api.openweathermap.org"

const uint16_t port = 443;

// not valid before: 2024-12-17 00:00:00
// not valid after:  2026-01-17 23:59:59
const char fingerprint_imgw_pl [] PROGMEM = "A4:0D:E5:C1:A2:AB:BE:18:92:43:0E:F0:B8:FB:13:90:79:63:53:C9";

// not valid before: 2025-01-23 23:28:13
// not valid after:  2025-04-24 00:28:10
const char fingerprint_w3_org [] PROGMEM = "87:66:BE:7A:E6:91:4B:71:A8:D4:56:20:84:27:1F:9A:E6:B1:50:B5";

// not valid before: 2024-07-19 00:00:00
// not valid after:  2025-03-21 23:59:59
const char fingerprint_open_weather_map_org [] PROGMEM = "10:6B:FC:4F:45:DE:49:52:E3:AD:F6:EB:D1:1E:C9:35:29:6F:43:D7";