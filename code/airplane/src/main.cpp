#include <Arduino.h>

#define PIN_STROBE 27
#define PIN_NAV 14

unsigned long strobe_millis = 0;
int step = 0;

void setup() {
  pinMode(PIN_STROBE, OUTPUT);
  pinMode(PIN_NAV, OUTPUT);

  digitalWrite(PIN_NAV, HIGH);
}

void loop() {
  unsigned long current_millis = millis();

  if(step == 0 && current_millis - strobe_millis >= 1000)
  {
    digitalWrite(PIN_STROBE, HIGH);
    strobe_millis = current_millis;
    step++;
  }

  if(step == 1 && current_millis - strobe_millis >= 50)
  {
    digitalWrite(PIN_STROBE, LOW);
    strobe_millis = current_millis;
    step++;
  }

  if(step == 2 && current_millis - strobe_millis >= 75)
  {
    digitalWrite(PIN_STROBE, HIGH);
    strobe_millis = current_millis;
    step++;
  }

  if(step == 3 && current_millis - strobe_millis >= 50)
  {
    digitalWrite(PIN_STROBE, LOW);
    strobe_millis = current_millis;
    step = 0;
  }
}