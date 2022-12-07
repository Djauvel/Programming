#include <Wire.h>
#include <Zumo32U4.h>

char commands[]=        {'f' ,'b' ,'l' ,'r' ,'s' }; //Commands Forward, Backward, Left, Right, Stop
int speedTime[][5]=    {{100  ,100  ,100  ,100   ,0  }, //Speeds for respective commands
                        {2000,1000,1000,1000,2000}}; //Durations for each command 
int nCom = 5;

Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  buttonA.waitForPress(); //Wait for buttonA to execute loop:
    for (int i=0; i<nCom; i++){
      Serial.println(commands[i]);
      switch (commands[i]){
        case 'f':
          motors.setSpeeds(speedTime[0][i],speedTime[0][i]); //forwards has two positive values
          delay(speedTime[1][i]);
          motors.setSpeeds(0,0);
          Serial.println("Speed = " + (String)speedTime[0][i] + " + " + "Time (in ms) = " + speedTime[1][i]);         //Prints speed and duration of command
        break;
        
        case 'b':
          motors.setSpeeds(-speedTime[0][i],-speedTime[0][i]); //negative values for motor speeds to go backwards
          delay(speedTime[1][i]);
          motors.setSpeeds(0,0);
          Serial.println("Speed = " + (String)speedTime[0][i] + " + " + "Time (in ms) = " + speedTime[1][i]);         //Prints speed and duration of command
        break;
        
        case 'l':
          motors.setSpeeds(-speedTime[0][i],speedTime[0][i]); //left motor is negative to turn left
          delay(speedTime[1][i]);
          motors.setSpeeds(0,0);
          Serial.println("Speed = " + (String)speedTime[0][i] + " + " + "Time (in ms) = " + speedTime[1][i]);         //Prints speed and duration of command
        break;
        
        case 'r':
          motors.setSpeeds(speedTime[0][i],-speedTime[0][i]); //right motor is negative to turn right
          delay(speedTime[1][i]);
          motors.setSpeeds(0,0);
          Serial.println("Speed = " + (String)speedTime[0][i] + " + " + "Time (in ms) = " + speedTime[1][i]);         //Prints speed and duration of command
        break;
        
        default:
           motors.setSpeeds(0,0);
          delay(speedTime[1][i]);
          Serial.println("Speed = " + (String)speedTime[0][i] + " + " + "Time (in ms) = " + speedTime[1][i]);
        }
    }
    delay(500); 
}
