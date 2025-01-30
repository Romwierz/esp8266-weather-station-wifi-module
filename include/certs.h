#pragma once

#define METEO_IMGW  "meteo.imgw.pl"
#define W3          "jigsaw.w3.org"

const uint16_t port = 443;

// not valid before: 2024-12-17 00:00:00
// not valid after:  2026-01-17 23:59:59
const char fingerprint_imgw_pl [] PROGMEM = "A4:0D:E5:C1:A2:AB:BE:18:92:43:0E:F0:B8:FB:13:90:79:63:53:C9";

// not valid before: 2025-01-23 23:28:13
// not valid after:  2025-04-24 00:28:10
const char fingerprint_w3_org [] PROGMEM = "87:66:BE:7A:E6:91:4B:71:A8:D4:56:20:84:27:1F:9A:E6:B1:50:B5";