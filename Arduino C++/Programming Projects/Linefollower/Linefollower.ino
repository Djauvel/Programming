#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LineSensors lineSensors;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;

//int searchLeft;
//int searchRight;
bool crossedfirst = false;
bool crossedsecond = false;
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
LineSensorsWhite sensorState = { 0,0,0,0,0 };

void setup() {
    lineSensors.initFiveSensors(); // Initializes five sensors
    readSensors(sensorState);      //fills structure with first bools
    Serial.begin(9600);
    calibrWht();                   //Calibrates the sensors for white lines
}

void loop() { 
  buttonA.waitForPress();         // waits to start until after calibration, to prevent eternal looping
  motors.setSpeeds(50,50);        // sets low speeds on motors after beginning
  while(!crossedsecond){          //until we have crossed the second line execute
     readSensors(sensorState);    //fill sensordata
     
    Serial.println("------------- SENSOR DATA -------------");
    Serial.println("Left sensor value:         " + (String)sensorValues[0] + ", state: " + (String)sensorState.left);
    Serial.println("Left Center sensor value:  " + (String)sensorValues[1] + ", state: " + (String)sensorState.leftCenter);
    Serial.println("Center sensor value:       " + (String)sensorValues[2] + ", state: " + (String)sensorState.Center);
    Serial.println("Right Center sensor value: " + (String)sensorValues[3] + ", state: " + (String)sensorState.rightCenter);
    Serial.println("RIGHT sensor value:        " + (String)sensorValues[4] + ", state: " + (String)sensorState.right);
    Serial.println("------------- END OF DATA -------------"); //print sensordata
    
    if (sensorState.left && !crossedfirst) //If sensorState bool on the left sensor is true and if we haven't crossed the first line
    {
        Serial.println("Crossed the first line"); //Print
        crossedfirst = true;                      //Set the bool of crossing the first line to true
        motors.setSpeeds(100, 100);               //Begin to drive faster toward second line
        delay(2000);
    }
    readSensors(sensorState);
    if (crossedfirst && sensorState.left)   //if sensor we have crossed the first line, and if left sensorstate is true
    {
      Serial.println("Just crossed 2nd line"); //print
      delay(200); // wait for zumo to cross over the line a bit
      motors.setSpeeds(0, 0); // stop engines after crossing first line
        
    }
  }
}

void calibrWht() { //A funtion that is called in the setup to help calibrate sensors for the conditions at hand
    buttonA.waitForPress();
    Serial.println("Press A to calibrate WHITE");
    delay(250);
    buttonA.waitForPress();
    threshold = ((sensorValues[0] + sensorValues[4]) / 2 + 20); //takes the mean value of far left and right sensors and adds some margin to create a threshold
    delay(250);
    Serial.println(threshold); //prints threshold once at the beginning of the code
}

void readSensors(LineSensorsWhite& state) {  // Next line reads the sensor values and store them in the array lineSensorValues
    lineSensors.read(sensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF); //Retrieves data from sensors
    state = { 0,0,0,0,0 }; // state of the sensors is ALWAYS set to negative in the structure, so that the if statements below only change the boolean to true when the conditions are met
    if (sensorValues[0] < threshold) {
        state.left = 1;
    }
    if (sensorValues[1] < threshold) {
        state.leftCenter = 1;
    }
    if (sensorValues[2] < threshold) {
        state.Center = 1;
    }
    if (sensorValues[3] < threshold) {
        state.rightCenter = 1;
    }
    if (sensorValues[4] < threshold) {
        state.right = 1;
    }
}
