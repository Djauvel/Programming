#include <Wire.h>
#include <Zumo32U4.h>

char commands[]=        {'f' ,'b' ,'l' ,'r' ,'s' }; //Commands Forward, Backward, Left, Right, Stop
int speedTime[][5]=    {{100  ,100  ,100  ,100   ,0  }, //Speeds for respective commands
                        {2000,1000,1000,1000,2000}}; //Durations for each command 
int nCom = 5;

Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;

void moveForward(int i){
            motors.setSpeeds(speedTime[0][i],speedTime[0][i]);
}
void moveBackward(int i){
            motors.setSpeeds(-speedTime[0][i],-speedTime[0][i]);
}
void moveLeft(int i){
            motors.setSpeeds(-speedTime[0][i],speedTime[0][i]);
}
void moveRight(int i){
            motors.setSpeeds(speedTime[0][i],-speedTime[0][i]);
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
          moveForward(i);
          Serial.println("Speed = " + (String)speedTime[0][i] + " + " + "Time (in ms) = " + speedTime[1][i]);         //Prints speed and duration of command
        break;
        
        case 'b':
         moveBackward(i);
          Serial.println("Speed = " + (String)speedTime[0][i] + " + " + "Time (in ms) = " + speedTime[1][i]);         //Prints speed and duration of command
        break;
        
        case 'l':
         moveLeft(i);
          Serial.println("Speed = " + (String)speedTime[0][i] + " + " + "Time (in ms) = " + speedTime[1][i]);         //Prints speed and duration of command
        break;
        
        case 'r':
         moveRight(i);
          Serial.println("Speed = " + (String)speedTime[0][i] + " + " + "Time (in ms) = " + speedTime[1][i]);         //Prints speed and duration of command
        break;
        
        default:
           motors.setSpeeds(0,0);
          Serial.println("Speed = " + (String)speedTime[0][i] + " + " + "Time (in ms) = " + speedTime[1][i]);
        }
        delay(speedTime[1][i]);
    }
    delay(500); 
}
