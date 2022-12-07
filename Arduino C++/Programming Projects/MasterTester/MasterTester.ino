  #include <Wire.h>
  #include <Zumo32U4.h>

  Zumo32U4Motors motors;
  Zumo32U4LCD lcd;
  Zumo32U4ButtonA buttonA;
  Zumo32U4ButtonB buttonB;
  Zumo32U4ButtonC buttonC;
  Zumo32U4IMU imu;
  Zumo32U4LineSensors lineSensors;

  bool pause = true; //Pause to prevent it starting before adjusting settings
  bool findLine = false;
  bool findIR = false;


  
  float desired_lin_speed = 75.0;
  float rot_gain = 0.1; // Tip: This gain could be more agressive.

  const uint8_t NUM_SENSORS = 5; //Number of activated sensors
  uint16_t sensorValues[NUM_SENSORS]; //Some array that contains the raw read values from the sensors between 0-2000
  bool useEmitters = true;
  
  struct LineSensorsWhite { //Datatype that stores the boolean values for the sensorStates
    bool left;
    bool leftCenter;
    bool Center;
    bool rightCenter;
    bool right;
  };
  
  struct turnCounter{
    int turns;
  };
  
  int threshold; // White threshold, white return values lower than this... Having some issues with boolean returns on sensorstates--
  int threshold2; // white treshold for center 3 sensors
  LineSensorsWhite sensorState = {false,false,false,false,false};

  void setup() {
    lineSensors.initFiveSensors();
    readSensors(sensorState);
    loadCustomCharacters();

    calibrWht();

    Serial.begin(9600);
    
    lcd.clear();
    lcd.print(F("Press A"));
    lcd.gotoXY(0, 1);
    lcd.print(F("to calib"));
    buttonA.waitForButton();

    lcd.clear();
    lcd.gotoXY(0,1);
    lcd.print(F("A->Cont."));
    while (!buttonA.getSingleDebouncedPress()) {
      lineSensors.calibrate();
      lineSensors.readCalibrated(sensorValues);
      printReadingsToLCD(sensorValues);
    }
  }

  void loop() {
    
   while(!findLine){
      motors.setSpeeds(75,75);
      readSensors(sensorState);
      
      if(sensorState.left && sensorState.right){
        findLine = true;
        motors.setSpeeds(0,0);
        }
        delay(100);
      }

  while(findLine){
    lineSensors.readCalibrated(sensorValues);
    printReadingsToLCD(sensorValues);
    int line_position = lineSensors.readLine(sensorValues);
  

  // Negating the line position, because the positive rotation is defined
  // counter-clockwise and the line position is clock-wise.
  float rot_speed = rot_gain * -(line_position - 2000);
  float lin_speed = desired_lin_speed;
  if (pause) {
    setLinRotSpeeds(0., 0.);
  } else {
    setLinRotSpeeds(lin_speed, rot_speed);
  }
  
  if (pause) {
    lcd.gotoXY(0,1);
    lcd.print(F(" Paused "));
  } else {
    lcd.gotoXY(0,1);
    lcd.print(F("        "));
    lcd.gotoXY(0,1);
    lcd.print((int)lin_speed);
    lcd.print(' ');
    lcd.print((int)rot_speed);
  }

  // If button A is pressed, pause.
  if (buttonA.getSingleDebouncedPress())
  {
    pause = !pause;
  }

  // If button B is pressed, decrease speed.
  if (buttonB.getSingleDebouncedPress())
  {
    desired_lin_speed *= 0.9;
  }

  // If button C is pressed, increase speed.
  if (buttonC.getSingleDebouncedPress())
  {
    desired_lin_speed *= 1.1;
  }
    readSensors(sensorState);
    Serial.println("------------- SENSOR DATA -------------");
    Serial.println("Left sensor value:         " + (String)sensorValues[0] + ", state: " + (String)sensorState.left);
    Serial.println("Left Center sensor value:  " + (String)sensorValues[1] + ", state: " + (String)sensorState.leftCenter);
    Serial.println("Center sensor value:       " + (String)sensorValues[2] + ", state: " + (String)sensorState.Center);
    Serial.println("Right Center sensor value: " + (String)sensorValues[3] + ", state: " + (String)sensorState.rightCenter);
    Serial.println("RIGHT sensor value:        " + (String)sensorValues[4] + ", state: " + (String)sensorState.right);
    Serial.println("------------- END OF DATA -------------"); //print sensordata
    
  }
   motors.setSpeeds(0,0);
   Serial.println("Crossed the line");
   delay(200);
   
  }

  //Functions;
  
  void calibrWht() { //A funtion that is called in the setup to help calibrate sensors for the conditions at hand
      buttonA.waitForPress();
      Serial.println("Press A to calibrate WHITE");
      delay(250);
      buttonA.waitForPress();
      threshold = ((sensorValues[0]  + sensorValues[4]) / 2 + 20); //takes the mean value of far left and right sensors and adds some margin to create a threshold
      threshold2 = ((sensorValues[1] + sensorValues[2] + sensorValues[3])/3 + 20); 
      delay(250);
      Serial.println(threshold); //prints threshold once at the beginning of the code
      Serial.println(threshold2);
  }

  void readSensors(LineSensorsWhite& state) {  // Next line reads the sensor values and store them in the array lineSensorValues
      lineSensors.read(sensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF); //Retrieves data from sensors
      state = {false,false,false,false,false}; // state of the sensors is ALWAYS set to negative in the structure, so that the if statements below only change the boolean to true when the conditions are met
      if (sensorValues[0] > threshold) {
          state.left = true;
      }
      if (sensorValues[1] > threshold2) {
          state.leftCenter = true;
      }
      if (sensorValues[2] > threshold2) {
          state.Center = true;
      }
      if (sensorValues[3] > threshold2) {
          state.rightCenter = true;
      }
      if (sensorValues[4] > threshold) {
          state.right = true;
      }
  }

  void loadCustomCharacters()
  {
    static const char levels[] PROGMEM = {
      0, 0, 0, 0, 0, 0, 0, 63, 63, 63, 63, 63, 63, 63
    };
    for (int i = 0; i < 7; ++i) {
      lcd.loadCustomCharacter(levels + i, i);
    }
  }
  
  void printBar(uint8_t height)
  {
    if (height > 8) { height = 8; }
    static const char barChars[] = {' ', 0, 1, 2, 3, 4, 5, 6, '\xff'};
    lcd.print(barChars[height]);
  }
  
  void printReadingsToLCD(unsigned int* readings)
  {
    lcd.gotoXY(0, 0); // Modify this, if you want to change the position.
    for (uint8_t i = 0; i < 5; i++)
    {
      uint8_t barHeight = map(readings[i], 0, 1000, 8, 0);
      printBar(barHeight);
    }
  }

  void setLinRotSpeeds(float lin_speed, float rot_speed) {
  float left_speed = lin_speed - rot_speed;
  float right_speed = lin_speed + rot_speed;
  // Now, constrain the speeds on both tracks, so that angular velocity is
  // prioritized.
  if (left_speed > 400) {
    right_speed = right_speed - (left_speed - 400);
    if (right_speed < -400) {right_speed = -400;}
    left_speed = 400;
  } else if (right_speed > 400) {
    left_speed = left_speed - (right_speed - 400);
    if (left_speed < -400) {left_speed = -400;}
    right_speed = 400;
  } else if (left_speed < -400) {
    right_speed = right_speed - (left_speed + 400);
    if (right_speed > 400) {right_speed = 400;}
    left_speed = -400;
  } else if (right_speed < -400) {
    left_speed = left_speed - (right_speed + 400);
    if (left_speed > 400) {left_speed = 400;}
    right_speed = -400;
  }

  motors.setSpeeds(left_speed, right_speed);
}
