
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Encoders encoders;
Zumo32U4Motors motors;

void setup() {
  Serial.begin(9600);
}

void loop() {
    float countsL = encoders.getCountsLeft();
   // double movementIncm = (countsL / 900) * (PI * 4) - 1.5;
   Serial.println((String)countsL);
   delay(100);
  }
