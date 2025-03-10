# Weather Station ESP8266 Firmware

This repository contains the firmware for the ESP8266 used in [this](https://github.com/Romwierz/weather-station) STM32 Weather Station project. 

The ESP8266 connects to the Internet, retrieves weather data and communicates with the STM32 via UART.

The OpenWeatherMap API provides various weather data. The API key can be obtained for free from [this](https://openweathermap.org/price#freeaccess) site.

## Installation
1. Clone this repository using Git command:
   ```bash
   git clone https://github.com/Romwierz/esp8266-weather-station-wifi-module.git
    ```
2. Open project folder (the one with `platformio.ini` inside it) through PlatformIO (e.g. as VSCode extension).
3. Set up the environment:
    - Choose, modify or add a proper **configuration environment** in `platformio.ini` to match your specific ESP8266 board/module. ([more info](https://docs.platformio.org/en/latest/projectconf/sections/env/index.html))
    - Specify the correct **upload_port**. ([more info](https://docs.platformio.org/en/latest/projectconf/sections/env/options/upload/upload_port.html))
4. Add your WiFi credentials and OpenWeatherMap API key inside `data/config.txt`.
5. **Upload Filesystem Image** within **PIO Project Tasks**.
5. **Build** and **Upload** the firmware.