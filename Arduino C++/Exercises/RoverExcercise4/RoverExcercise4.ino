#include <Wire.h>
#include <Zumo32U4.h>


struct roverControl
{
    char command;
    int speed;
    int distance;
};


int nCom = 8;
int drivenDist = 0;
roverControl commands[8] = {};  //Commands Forward, Backward, Left, Right, Stop


Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;

void setup() {
    for (int i = 0; i < nCom; i++) {
        commands[i].command = 'f';
        commands[i].speed = 80+ 10*i;
        commands[i].distance = 15;
    }
}

void loop() {
    

  buttonA.waitForPress(); //Wait for buttonA to execute loop:
  delay(250);             //wait for user to move finger from button
  
    for (int i=0; i<nCom; i++){
      
      switch (commands[i].command) {
        case rc.command == 'f':
        moveForward(rc.speed,rc.distance,i);
        break;
        
        case 'b':
         moveBackward(speedDist[0][i],speedDist[1][i],i);
        break;
        
        case 'l':
         moveLeft(speedDist[0][i],speedDist[1][i],i);
        break;
        
        case 'r':
         moveRight(speedDist[0][i],speedDist[1][i],i);
        break;
        
        default:
           motors.setSpeeds(0,0);
        }
    }
}

void moveForward(int speeds,int dist,int iteration){                //Functions for moving directions, expecting three values. One for speed and one for time
            while(drivenDist<speedDist[1][iteration]){              //while desired distance travelled is greater than the actually travelled distance
            float countsL = encoders.getCountsLeft();               //Retrieve motorcounts
            double movementIncm = (countsL / 900) * (PI * 4) - 1.5; //Convert motorcounts to CM            
            motors.setSpeeds(speeds,speeds);                        //Set positive speeds to go forward
            drivenDist = movementIncm;
            }
            motors.setSpeeds(0,0);                                  //turn off motors
            encoders.getCountsAndResetLeft();                       //Reset encodercount for future use
            drivenDist = 0;
}
void moveBackward(int speeds,int dist,int iteration){               //Functions for moving directions, expecting three values. One for speed and one for time and one for the iteration number from for-loop
            while(drivenDist<speedDist[1][iteration]){              //while desired distance travelled is greater than the actually travelled distance
            float countsL = encoders.getCountsLeft();               //Retrieve motorcounts
            double movementIncm = (-countsL / 900) * (PI * 4) - 1.5;//Convert motorcounts to CM (countsL is negative to get positive value)
            motors.setSpeeds(-speeds,-speeds);                      //Set positive speeds to go forward
            drivenDist = movementIncm;
            }
            motors.setSpeeds(0,0);                                  //turn off motors
            encoders.getCountsAndResetLeft();                       //Reset encodercount for future use
            drivenDist = 0;
}
void moveLeft(int speeds,int dist,int iteration){                   //Functions for moving directions, expecting three values. One for speed and one for time and one for the iteration number from for-loop
            while(drivenDist<speedDist[1][iteration]){              //while desired distance travelled is greater than the actually travelled distance
            float countsL = encoders.getCountsLeft();               //Retrieve motorcounts (on the right motor this time because motorcounts cannot be negative in following conversion)
            double movementIncm = (-countsL / 900) * (PI * 4) - 1.5;//Convert motorcounts to CM
            motors.setSpeeds(-speeds,speeds);                       //Set left side to negative to turn left
            drivenDist = movementIncm;
            }
            motors.setSpeeds(0,0);                                  //turn off motors
            encoders.getCountsAndResetLeft();                       //Reset encodercount for future use
            drivenDist = 0;
}
void moveRight(int speeds,int dist,int iteration){                  //Functions for moving directions, expecting three values. One for speed and one for time and one for the iteration number from for-loop
            while(drivenDist<speedDist[1][iteration]){              //while desired distance travelled is greater than the actually travelled distance
            float countsL = encoders.getCountsLeft();               //Retrieve motorcounts
            double movementIncm = (countsL / 900) * (PI * 4) - 1.5; //Convert motorcounts to CM
            motors.setSpeeds(speeds,-speeds);                       //Set right side to negative to turn right
            drivenDist = movementIncm;
            }
            motors.setSpeeds(0,0);                                  //Turn off motors
            encoders.getCountsAndResetLeft();                       //Reset encodercount for future use
            drivenDist = 0;
}
