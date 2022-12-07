#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;

struct roverControl
{
    char comm;
    int spd;
    int distance;
};


int nCom = 1;
int drivenDist = 0;
roverControl commands[1] = {};  //Commands Forward, Backward, Left, Right, Stop

void setup() {
  
        commands[0].comm = 'f';
        commands[0].spd = 400;
        commands[0].distance = 45;
/*
        commands[1].comm = 'l';
        commands[1].spd = 200;
        commands[1].distance = 5;

        commands[2].comm = 'b';
        commands[2].spd = 200;
        commands[2].distance = 15;

        commands[3].comm = 'r';
        commands[3].spd = 200;
        commands[3].distance = 5;

        commands[4].comm = 'f';
        commands[4].spd = 200;
        commands[4].distance = 15;
        */
}

void loop() {

    buttonA.waitForPress(); //Wait for buttonA to execute loop:
    delay(250);             //wait for user to move finger from button

    for (int i = 0; i < nCom; i++) {

        switch (commands[i].comm) {
        case 'f':
            moveForward(commands[i].spd, commands[i].distance);
            break;

        case 'b':
            moveBackward(commands[i].spd, commands[i].distance);
            break;

        case 'l':
            moveLeft(commands[i].spd, commands[i].distance);
            break;

        case 'r':
            moveRight(commands[i].spd, commands[i].distance);
            break;

        default:
            motors.setSpeeds(0, 0);
        }
        delay(500);
    }
}

void moveForward(int speeds, int dist) {                //Functions for moving directions, expecting three values. One for speed and one for time
    while (drivenDist < dist) {              //while desired distance travelled is greater than the actually travelled distance
        float countsL = encoders.getCountsLeft();               //Retrieve motorcounts
        double movementIncm = (countsL / 900) * (PI * 4) - 1.5; //Convert motorcounts to CM            
        motors.setSpeeds(speeds, speeds);                        //Set positive speeds to go forward
        drivenDist = movementIncm;
    }
    motors.setSpeeds(0, 0);                                  //turn off motors
    encoders.getCountsAndResetLeft();                       //Reset encodercount for future use
    drivenDist = 0;
}
void moveBackward(int speeds, int dist) {               //Functions for moving directions, expecting three values. One for speed and one for time and one for the iteration number from for-loop
    while (drivenDist < dist) {              //while desired distance travelled is greater than the actually travelled distance
        float countsL = encoders.getCountsLeft();               //Retrieve motorcounts
        double movementIncm = (-countsL / 900) * (PI * 4) - 1.5;//Convert motorcounts to CM (countsL is negative to get positive value)
        motors.setSpeeds(-speeds, -speeds);                      //Set positive speeds to go forward
        drivenDist = movementIncm;
    }
    motors.setSpeeds(0, 0);                                  //turn off motors
    encoders.getCountsAndResetLeft();                       //Reset encodercount for future use
    drivenDist = 0;
}
void moveLeft(int speeds, int dist) {                   //Functions for moving directions, expecting three values. One for speed and one for time and one for the iteration number from for-loop
    while (drivenDist < dist) {              //while desired distance travelled is greater than the actually travelled distance
        float countsL = encoders.getCountsLeft();               //Retrieve motorcounts (on the right motor this time because motorcounts cannot be negative in following conversion)
        double movementIncm = (-countsL / 900) * (PI * 4) - 1.5;//Convert motorcounts to CM
        motors.setSpeeds(-speeds, speeds);                       //Set left side to negative to turn left
        drivenDist = movementIncm;
    }
    motors.setSpeeds(0, 0);                                  //turn off motors
    encoders.getCountsAndResetLeft();                       //Reset encodercount for future use
    drivenDist = 0;
}
void moveRight(int speeds, int dist) {                  //Functions for moving directions, expecting three values. One for speed and one for time and one for the iteration number from for-loop
    while (drivenDist < dist) {              //while desired distance travelled is greater than the actually travelled distance
        float countsL = encoders.getCountsLeft();               //Retrieve motorcounts
        double movementIncm = (countsL / 900) * (PI * 4) - 1.5; //Convert motorcounts to CM
        motors.setSpeeds(speeds, -speeds);                       //Set right side to negative to turn right
        drivenDist = movementIncm;
    }
    motors.setSpeeds(0, 0);                                  //Turn off motors
    encoders.getCountsAndResetLeft();                       //Reset encodercount for future use
    drivenDist = 0;
}
