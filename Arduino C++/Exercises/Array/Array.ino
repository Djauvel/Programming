#include <Wire.h> // libraries needed to use zumo functions
#include <Zumo32U4.h>
Zumo32U4Motors motors; // instanciate the motors
Zumo32U4ButtonA buttonA;

int mSpeed = 100;

void setup() { 
  Serial.begin(9600);
  buttonA.waitForPress();
}
void loop() {
motors.setSpeeds(0, 0); // turn off mottors
delay(1000);
motors.setSpeeds(mSpeed,-mSpeed); // set L and R speeds , minus speed for backwards.
delay(300);
motors.setSpeeds(-mSpeed,mSpeed);
delay(300);
}
/*
//long SexyNumbers[1][5]={{420,69,1337,80085,666},{1,2,3,4,5}};

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;

int mSpeed = 100;

void setup() { 
  Serial.begin(9600);
  buttonA.waitForPress();
}

void loop() {
  set

  
 /* for (int i=0; i<5; i++)
  {
    for (int j=0; j<5; j++)
    {
      Serial.println("Line " + (String)i + "," + "column"  + (String)j + " = " + (String)SexyNumbers[i][j]);
      //SexyNumbers[i]= SexyNumbers[i]*2;
     }
    }
    
    //int nElements = sizeof(SexyNumbers)/sizeof(5);
    //Serial.println("The array uses " + (String)nElements + " bytes of memory");
    Serial.println("---- New Loop ----");
    delay(5000);
    */
