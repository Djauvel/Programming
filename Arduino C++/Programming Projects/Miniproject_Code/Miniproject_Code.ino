/*
 Name:    main.ino
 Created: 10/25/2021 11:46:02 AM
 Authors: Nikolaj Krebs, Peter Plass
 Group:   B378a
*/

#include <Zumo32U4.h>

//// Motor Control
#define MOTOR_SPEED 100
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;
bool rotating = false;
bool calLineFound = false;

/// LCD & Buttons
Zumo32U4LCD lcd;
Zumo32U4ButtonA aButton;
Zumo32U4ButtonB bButton;
Zumo32U4ButtonC cButton;

/// Sensors
#define NUM_SENSORS 5
#define BRIGHTNESS_LEVEL_COUNT 8
#define leftSensor 0
#define leftCenterSensor 1
#define centerSensor 2
#define rightCenterSensor 3
#define rightSensor 4

/// <summary>
/// Represents whether each invididual line/IR sensor on the belly of the Zumo32U4 robot detects a specific
/// contrast that is defined by the threshold in main.readSensorValues(). This struct also
/// contains functions for getting and setting sensor states externally. setState(int, bool) and getState(int)
/// </summary>
struct sensorDetectionState
{ 
  // L: Left, LC: Left Center, C: Center, RC: Right Center, R: Right
  // 
  //                 L      LC     C      RC     R
  bool states[NUM_SENSORS] = { false, false, false, false, false };

  void setState(int sensorId, bool val)
  {
    states[sensorId] = val;
  }

  bool getState(int sensorId)
  {
    return states[sensorId];
  }
};

bool useEmitter = true;
int sensorThresholds[NUM_SENSORS];

uint16_t brightnessLevels[BRIGHTNESS_LEVEL_COUNT]; // The span of measurements representing the amount of light response.
uint16_t sensorReadValues[NUM_SENSORS];
Zumo32U4ProximitySensors proxSensors;
Zumo32U4LineSensors lineSensors;
sensorDetectionState sensorStates = { false, false, false, false, false };

/// Debugging
#define SERIAL_FREQUENCY 9600



// the setup function runs once when you press reset or power the board
void setup() 
{
  configureComponents();
  delay(250);
}

// the loop function runs over and over again until power down or reset
void loop() 
{

}

/// <summary>
/// Sets all the basic values and properties for components on the robot.
/// </summary>
void configureComponents()
{
  Serial.begin(SERIAL_FREQUENCY);
  lcd.clear();

  for (int i = 1; i < BRIGHTNESS_LEVEL_COUNT; i++)
  {
    brightnessLevels[i] = i;
  }
  /// <summary>
  /// Creates three threshold values for the linesensor pairs. 
  /// They are divided into pairs because the center sensors and outer-most sensors usually read different values.
  /// </summary>
  int thresVal0 = ((sensorReadValues[0] + sensorReadValues[4]) / 2 + 20);
  int thresVal1 = ((sensorReadValues[1] + sensorReadValues[3]) / 2 + 20);
  int thresVal2 = (sensorReadValues[2] + 20);

  sensorThresholds[0] = thresVal0;
  sensorThresholds[1] = thresVal1;
  sensorThresholds[2] = thresVal2;
  sensorThresholds[3] = thresVal1;
  sensorThresholds[4] = thresVal0;

  proxSensors.initThreeSensors();
  proxSensors.setBrightnessLevels(brightnessLevels, BRIGHTNESS_LEVEL_COUNT);
  lineSensors.initFiveSensors();
  calibrateLineSensors();
}

/// <summary>
/// Calibrates the line sensors to follow white lines on a dark background.
/// </summary>
void calibrateLineSensors()
{
  aButton.waitForPress();
  lcd.print("Press A");
  delay(250); // Is this needed?
  aButton.waitForPress();
  readSensorValues(sensorStates);
  delay(250); // Is this needed?
  lcd.clear();
  setSpeed(0.75);
}

/// <summary>
/// Handles line sensor detection states by going through the total num
/// of sensors where it afterwards sets the specific sensors read value to the 
/// respected value based on wether it exceeds a read above or below the threshold.
/// </summary>
/// <param name="state"></param>
void readSensorValues(sensorDetectionState& state)
{
  bool emit = useEmitter ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF;
  lineSensors.read(sensorReadValues, emit);
  state = { false, false, false, false, false };

  for (int i = 0; i < NUM_SENSORS; i++)
  {
    if (sensorReadValues[i] < sensorThresholds[i])
    {
      state.setState(i, true);
    }
  }
}

/// <summary>
/// Moves the robot in a forward motion to find the calibration line.
/// </summary>
void findCalLine()
{
  while (!calLineFound)
  {

  }
}

/// <summary>
/// Set's the speed on both motors to a common speed as long as speed2 is not defined.
/// If speed2 is defined the order of the arguments will set the speed to it's respective motor.
/// </summary>
/// <param name="speed"></param>
void setSpeed(int16_t speed, int16_t speed2 = NULL)
{
  int16_t dSpd = speed2 != NULL ? speed2 : speed;
  motors.setSpeeds(speed, dSpd);
}

/// <summary>
/// Turns the robot in a left direction based on a desired angle different from 0 degrees.
/// </summary>
/// <param name="angle"></param>
void turnLeft(float angle)
{

}

/// <summary>
/// Turns the robot in a right direction based on a desired angl different from 0 degrees.
/// </summary>
/// <param name="angle"></param>
void turnRight(float angle)
{

}

/// <summary>
/// TODO: Write something here.
/// </summary>
void stopMotors()
{
  motors.setSpeeds(0, 0);
}
