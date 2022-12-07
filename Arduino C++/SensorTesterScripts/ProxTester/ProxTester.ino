#include <Zumo32U4.h>
#include <Wire.h>

Zumo32U4LCD lcd;
Zumo32U4ProximitySensors proxSensors;
#define NUM_SENSORLEVELS 6
//uint16_t brightnessLevels[NUM_SENSORLEVELS]={1,1,3,3,5,5};
uint16_t brightnessLevels[]={1,1,3,3,5,5};

void setup() {
  // put your setup code here, to run once:
  proxSensors.initThreeSensors();
  //proxSensors.setBrightnessLevels(brightnessLevels,NUM_SENSORLEVELS/2); 
  proxSensors.setBrightnessLevels(brightnessLevels,sizeof(brightnessLevels)/2); 
}

void loop() {
  // put your main code here, to run repeatedly:
  proxSensors.read();
  int center_left_sensor = proxSensors.countsFrontWithLeftLeds();
  int center_right_sensor = proxSensors.countsFrontWithRightLeds();
  lcd.clear();
  lcd.gotoXY(0,0);
  lcd.print((String)center_left_sensor);
  lcd.gotoXY(0,1);
  lcd.print((String)center_right_sensor);
  delay(100);
}
