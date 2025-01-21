#include <Arduino.h>

// for ESP-01S it has to be changed to GPIO2
#define LED_BUILTIN 2

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(10);

  Serial.println("Hello there");
}

void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  Serial.println("I am alive");
  delay(500);
}
