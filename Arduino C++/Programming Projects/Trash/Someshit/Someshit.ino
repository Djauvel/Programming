#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4LCD lcd;
Zumo32U4Encoders encoders;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4ButtonA buttonA;


bool findLine = false;
bool findIR = false;


#define NUM_SENSORS 5 //Number of activated sensors
uint16_t sensorValues[NUM_SENSORS]; //Some array that contains the raw read values from the sensors between 0-2000
bool useEmitters = true;
struct LineSensorsWhite { //Datatype that stores the boolean values for the sensorStates
    bool left;
    bool leftCenter;
    bool Center;
    bool rightCenter;
    bool right;
};
int threshold; // White threshold, white return values lower than this
int threshold2; // white treshold for center 3 sensors
LineSensorsWhite sensorState = {false,false,false,false,false};


void setup() {
  Serial.begin(9600);
  lineSensors.initFiveSensors(); // Initializes five sensors
  calibrWht();
  Serial.println("Press A to start");
  buttonA.waitForPress();
}

void loop() {
    delay(250);
    
    
    while(!findLine){
      motors.setSpeeds(75,75);
      readSensors(sensorState);
      
      if(sensorState.left && sensorState.right){
        delay(200);
        findLine = true;
        motors.setSpeeds(0,0);
        }
        delay(100);
      }

    while(findLine && !findIR){ //
      motors.setSpeeds(100,-60);
      delay(1500);
      motors.setSpeeds(75,75);
      delay(900);
      findIR = true;
      }
    motors.setSpeeds(0,0);
     /*
    readSensors(sensorState);    //fill sensordata
     
    Serial.println("------------- SENSOR DATA -------------");
    Serial.println("Left sensor value:         " + (String)sensorValues[0] + ", state: " + (String)sensorState.left);
    Serial.println("Left Center sensor value:  " + (String)sensorValues[1] + ", state: " + (String)sensorState.leftCenter);
    Serial.println("Center sensor value:       " + (String)sensorValues[2] + ", state: " + (String)sensorState.Center);
    Serial.println("Right Center sensor value: " + (String)sensorValues[3] + ", state: " + (String)sensorState.rightCenter);
    Serial.println("RIGHT sensor value:        " + (String)sensorValues[4] + ", state: " + (String)sensorState.right);
    Serial.println("------------- END OF DATA -------------"); //print sensordata
    */
  //Drive forward to detect starting line

  //Follow line

  //stop at sensor

  //evaluate can type

  //execute a pushing function depending on the can type
    //Detect border line and stop

  //return to the starting line
  
}

//FUNCTIONS

void calibrWht() { //A funtion that is called in the setup to help calibrate sensors for the conditions at hand
    buttonA.waitForPress();
    Serial.println("Press A to calibrate WHITE");
    delay(250);
    buttonA.waitForPress();
    readSensors(sensorState);
    threshold = ((sensorValues[0] + sensorValues[4]) / 2 + 20); //takes the mean value of far left and right sensors and adds some margin to create a threshold
    threshold2 = ((sensorValues[1] + sensorValues[2] + sensorValues[3])/3 + 20); 
    delay(250);
    Serial.println(threshold); //prints threshold once at the beginning of the code
    Serial.println(threshold2);
}

void readSensors(LineSensorsWhite& state) {  // Next line reads the sensor values and store them in the array lineSensorValues
    lineSensors.read(sensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF); //Retrieves data from sensors
    state = {false,false,false,false,false}; // state of the sensors is ALWAYS set to negative in the structure, so that the if statements below only change the boolean to true when the conditions are met
    if (sensorValues[0] < threshold) {
        state.left = true;
    }
    if (sensorValues[1] < threshold2) {
        state.leftCenter = true;
    }
    if (sensorValues[2] < threshold2) {
        state.Center = true;
    }
    if (sensorValues[3] < threshold2) {
        state.rightCenter = true;
    }
    if (sensorValues[4] < threshold) {
        state.right = true;
    }
}
