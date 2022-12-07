
#include <Wire.h>
#include <Zumo32U4.h>
int drivenDist = 0;
int totalDist = 0;
float desiredDist = 20;
Zumo32U4ButtonA buttonA;
Zumo32U4Encoders encoders;
Zumo32U4Motors motors;
Zumo32U4LCD lcd;
void setup() {
  Serial.begin(9600);
}

void loop() {
  buttonA.waitForPress();
  delay(200);
  while(drivenDist<desiredDist){
    float countsL = encoders.getCountsLeft();
    double movementIncm = (countsL / 900) * (PI * 4) - 1.5;
    motors.setSpeeds(100,100);
    drivenDist = movementIncm;
    delay(20);
  }
  motors.setSpeeds(0,0);
  lcd.clear();
  totalDist = drivenDist + totalDist;
  lcd.print((String)totalDist);
  drivenDist = 0;
  encoders.getCountsAndResetLeft();
}
