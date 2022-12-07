int pinRMD = 15; //LOW drives the engine forward, HIGH drives the engine in reverse
int pinLMD = 16; //^^^^
int pinRMS = 9; //Controls right motor speed with PWM
int pinLMS = 10; //Controls left motor speed with PWM

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;

int MotorSpeed = 150;

void setup() {
  // put your setup code here, to run once:
 /* 
  pinMode(pinRMD, OUTPUT);
  pinMode(pinLMD, OUTPUT);
  pinMode(pinRMS, OUTPUT);
  pinMode(pinLMS, OUTPUT);
 */
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);
  buttonA.waitForButton();
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  ledYellow(1);
  for (int speed = 0; speed <= 400; speed++)
  {
    motors.setLeftSpeed(speed);
    delay(2);
    }
  }
