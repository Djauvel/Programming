#include <Wire.h>
#include <Zumo32U4.h>

char commands[]=        {'f' ,'b' ,'l' ,'r' ,'s' }; //Commands Forward, Backward, Left, Right, Stop
int speedTime[][5]=    {{100  ,100  ,100  ,100   ,0  }, //Speeds for respective commands
                        {2000,1000,1000,1000,2000}}; //Durations for each command 
int nCom = 5;

Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;

void moveForward(int speeds,int time1){
            motors.setSpeeds(speeds,speeds);
            delay(time1);
            Serial.println("Speed = " + (String)speeds + " + " + "Time (in ms) = " + time1);
            }
void moveBackward(int speeds,int time1){
            motors.setSpeeds(-speeds,-speeds);
            delay(time1);
            Serial.println("Speed = " + (String)speeds + " + " + "Time (in ms) = " + time1);;
}
void moveLeft(int speeds,int time1){
            motors.setSpeeds(-speeds,speeds);
            delay(time1);
            Serial.println("Speed = " + (String)speeds + " + " + "Time (in ms) = " + time1);
            }
void moveRight(int speeds,int time1){
            motors.setSpeeds(speeds,-speeds);
            delay(time1);
            Serial.println("Speed = " + (String)speeds + " + " + "Time (in ms) = " + time1);
}

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
          moveForward(speedTime[0][i],speedTime[1][i]);
        break;
        
        case 'b':
         moveBackward(speedTime[0][i],speedTime[1][i]);
        break;
        
        case 'l':
         moveLeft(speedTime[0][i],speedTime[1][i]);
        break;
        
        case 'r':
         moveRight(speedTime[0][i],speedTime[1][i]);
        break;
        
        default:
           motors.setSpeeds(0,0);
        }
    }
    delay(500); 
}
