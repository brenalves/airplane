#include <Arduino.h>

#include "Menu.h"

Menu menu;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("Testing...");

  menu.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  unsigned long currentMillis = millis();

  menu.update(currentMillis);
}