#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include <math.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4IMU imu;

//Booleans that help the robot know how far in the process it is
bool rotatCheck, reachedDest, traveling = false;
bool sensorreads, bounce = true;
////////////////////////Odometry Constants and variables/////////////////////////
int wheelDiameter = 4; //4 centimeters
int axleLength = 9; // 
int deltaT = 200;
double currentX, currentY, deltax, deltay = 0;
double globalTheta, momentTheta, goalTheta, deltaTheta, deltaD;
double velocityR, velocityL, fwdVelocity = 0;
//////////////////////////////////////////////////////

//////////////////////// MOTOR ////////////////////////
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;
#define MOTOR_SPEED 100
/////////////////////////////////////////////////////////

//////////////////////// SENSORS ////////////////////////
//Basic setup of settings and initializations of sensor arrays
#define NUM_SENSORS 5 //Number of activated sensors
#define brightnessSpan 6
Zumo32U4ProximitySensors proxSensors;
Zumo32U4LineSensors lineSensors;
uint16_t sensorValues[NUM_SENSORS]; //Some array that contains the raw read values from the sensors between 0-2000
uint16_t brightnessLevels[]={1,1,3,3,5,5};
bool useEmitters = true;

struct LineSensorsWhite //Datatype that stores the boolean values for the sensorStates
{
    bool left;
    bool leftCenter;
    bool Center;
    bool rightCenter;
    bool right;
};
LineSensorsWhite sensorState = { false,false,false,false,false };
////////////////////////////////////////////////////////////

struct destination
{
  int X;
  int Y;
  };
int currentGoal = -1;
destination goals[] = {{1000,1000},{2000,0},{1000,-1000},{0,0}};

//Storage values for linesensor thresholds and for distance measurement
int drivenDist, threshold, threshold2, threshold3;
// threshold: White threshold, white return values lower than this
// threshold2: white treshold for center 2 sensors
// threshold3: White threshold for center sensor

////////////////Gyro setup/////////////////////
int turnAngleDegrees, flippedturnAngleDegrees, turnAngle2;
uint32_t turnAngle = 0; 
uint16_t gyroLastUpdate = 0;
int16_t turnRate, gyroOffset;

////////Clock////////
  unsigned long previousMillis = 0;
  const long interval = 200;
  unsigned long currentMillis;
  
void setup() {
    //Calibrates various components and awaits a buttonpress to start
    configureComponents();
    lcd.print("prss A");
    buttonA.waitForPress();
    lcd.clear();
    delay(250);
    goTo(goals[0].X,goals[0].Y);
}

void loop() 
{
  //timer
  currentMillis = millis();
  updateAngle();
  if (0 < encoders.getCountsLeft() || 0 < encoders.getCountsRight()){
    updatePosition();
  }
}

/////////////////////// FUNCTIONS ///////////////////////
void updatePosition()
{
  updateAngleandDeltas();
  
  //local coordinates
  // Comparing robot orientation to the gain in x and y direction.
  if(globalTheta >= 0 && globalTheta <= 90)
  {
    if(deltay <= 0)
    {
      currentY = currentY + (-1*deltay);
    }
    else
    {
      currentY = currentY + deltay;
    }
    if(deltax <= 0)
    {
      currentX = currentX + (-1*deltax);
    }  
    else
    {
      currentX = currentX + deltax;
    }
  }
  
  if(globalTheta >= 90 && globalTheta < 180)
  {
    if(deltay <= 0)
    {
      currentY = currentY + (-1*deltay);
    }
    else
    {
      currentY = currentY + deltay;
    }
    if(deltax <= 0)
    {
      currentX = currentX + deltax;
    }  
    else
    {
      currentX = currentX + (-1*deltax);
    }    
  }
  
  if(globalTheta > 180 && globalTheta < 270)
  {
    if(deltay <= 0)
    {
      currentY = currentY + deltay;
    }
    else
    {
      currentY = currentY + (-1*deltay);
    }
    if(deltax <= 0)
    {
      currentX = currentX + deltax;
    }  
    else
    {
      currentX = currentX + (-1*deltax);
    }    
  }
  
  if(globalTheta > 270 && globalTheta < 360)
  {
    if(deltay <= 0)
    {
      currentY = currentY + deltay;
    }
    else
    {
      currentY = currentY + (-1*deltay);
    }
    if(deltax <= 0)
    {
      currentX = currentX + (-1*deltax);
    }  
    else
    {
      currentX = currentX + deltax;
    }    
  }
}

void updateAngleandDeltas()
{
  updateAngle();
  // motorChecked will be true only once about every 200ms, and will thus only update position once every 200ms
  if(motorChecked())
  {
  float encL = encoders.getCountsLeft(); 
  float encR = encoders.getCountsRight();
  fwdVelocity = (((encL + encR)/2)/900 * (PI * wheelDiameter));
  velocityR = ((encR)/900 * (PI * wheelDiameter));
  velocityL = ((encL)/900 * (PI * wheelDiameter));
  deltaD = (velocityR + velocityL)/2;
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
  deltax = (deltaD * cos(globalTheta*(PI/180)));
  deltay = (deltaD * sin(globalTheta*(PI/180)));
  
  //For debugging
  Serial.println("////////////////////////////////////////////////");
  Serial.println("Rv: " + (String)velocityR + " Lv: " + (String)velocityL);
  Serial.println("Velocity: " + (String)fwdVelocity + " cm");
  Serial.println("DeltaD: " + (String)deltaD);
  Serial.println("sin(globalTheta) = " + (String)sin(globalTheta*(PI/180)) + " cos(globalTheta) = " + (String)cos(globalTheta*(PI/180)));
  Serial.println("Theta:    " + (String)globalTheta);
  Serial.println("x:        " + (String)currentX);
  Serial.println("y:        " + (String)currentY);
  Serial.println("////////////////////////////////////////////////");

  lcd.clear();
  lcd.gotoXY(0,0);
  lcd.print("x: " + (String)currentX);
  lcd.gotoXY(0,1);
  lcd.print("y: " + (String)currentY);
  }
}

bool motorChecked()
{
  if (currentMillis - previousMillis >= interval) {
  previousMillis = currentMillis;
    return true;
  }
  else
  {
    return false;
  }
}

void configureComponents()
{
    for (int i = 1; i < brightnessSpan; i++)
    {
        brightnessLevels[i] = i;
    }
    turnAngle2 = turnAngle;
    Serial.begin(9600);
    proxSensors.initThreeSensors(); //Sets up proximity sensors
    proxSensors.setBrightnessLevels(brightnessLevels,sizeof(brightnessLevels)/2);  //Extends measurement levels for proximity sensors to increase overall accuracy

    turnSensorSetup(); //Sets up turning sensors
    delay(500);
    turnSensorReset();
    lcd.clear();

    lineSensors.initFiveSensors(); // Initializes five sensors
    // Uncomment this: calibrWht();
    Serial.println("Press A to start");
}

void goTo(int x, int y)
{
  traveling = true;
  int destx = x - currentX;
  int desty = y - currentY;
  int angleDeg = atan2(desty,destx)*180/PI;
  int angle = (angleDeg + 360) % 360;
   
  char dir = 0 < angle && angle < 180 ? 'L' : 'R';
  bool neg = angle < 0 ? true : false;
  bool destXNeg = x < 0 ? true : false;
  bool destYNeg = y < 0 ? true : false;
  turnGoal(angle,dir);
  reachedDest = false;
  moveForward(100,destx,desty,destXNeg,destYNeg);
}

bool verifyDestination(bool negX, bool negY, int x, int y)
{
    bool ret = false;
    if (negX && negY)
    {
        ret = currentX <= x && currentY <= y ? true : false;
    }
    else if (!negX && !negY)
    {
        ret = currentX >= x && currentY >= y ? true : false;
    }
    else if (!negX && negY)
    {
        ret = currentX >= x && currentY <= y ? true : false;
    }
    else if (negX && !negY)
    {
        ret = currentX <= x && currentY >= y ? true : false;
    }
  return ret;
}

void moveForward(int speed,int x,int y,bool destXneg, bool destYneg)
{
  motors.setSpeeds(speed,speed);
  while(!verifyDestination(destXneg,destYneg,x,y)&& !reachedDest)
  {
    currentMillis = millis();
    if (0 < encoders.getCountsLeft() || 0 < encoders.getCountsRight())
    {
      updatePosition();
    }
  }
  motors.setSpeeds(0,0);
  reachedDest = true;
  traveling = false;
  if(currentGoal<3)
  goTo(goals[currentGoal+1].X,goals[currentGoal+1].Y);
  currentGoal++;
}

void turnGoal(int goalAngle, char Direction)
{
  switch(Direction)
  {
    case 'R':
    updateAngle();

    while(!rotatCheck){
    updateAngle();
    if(globalTheta>=goalAngle&&(goalAngle)>=globalTheta)
    {
      motors.setSpeeds(0,0);
      rotatCheck=true;
    }
    else
    {
      motors.setSpeeds(MOTOR_SPEED,-MOTOR_SPEED * 1.2);
    }
    }
    rotatCheck = false;
    break;

    case 'L':
    updateAngle();
    while(!rotatCheck){
    updateAngle();
    if(globalTheta>=goalAngle&&(goalAngle)>=globalTheta)
    {
      motors.setSpeeds(0,0);
      rotatCheck=true;
    }
    else
    {
      motors.setSpeeds(MOTOR_SPEED,-MOTOR_SPEED * 1.2);
    }
    }
    rotatCheck = false;
    break;
  }
}

void turn(int desiredAngle, char Direction)
{
  switch(Direction)
  {
    case 'R':
    updateAngle();
    momentTheta = globalTheta;
    
      if(momentTheta - desiredAngle <= 0)
    {
      goalTheta = momentTheta - desiredAngle + 360;
    }
    else
    {
      goalTheta = momentTheta - desiredAngle ;
    }
    if(goalTheta == 360)
    {
      goalTheta = 0;
    }
    
    while(!rotatCheck){
    updateAngle();
    if(globalTheta>=goalTheta&&(goalTheta)>=globalTheta)
    {
      motors.setSpeeds(0,0);
      rotatCheck=true;
    }
    else
    {
      motors.setSpeeds(MOTOR_SPEED,-MOTOR_SPEED * 1.2);
    }
    }
    rotatCheck = false;
  break;

  case 'L':
    updateAngle();
    momentTheta = globalTheta;
    if(momentTheta + desiredAngle > 360)
    {
      goalTheta = momentTheta + desiredAngle - 360;
    }
    else
    {
      goalTheta = momentTheta + desiredAngle;
    }
    if(goalTheta == 360)
    {
      goalTheta = 0;
    }
  
    while(!rotatCheck){
    updateAngle();
    if(globalTheta>=goalTheta&&(goalTheta + 1)>=globalTheta)
    {
      motors.setSpeeds(0,0);
      rotatCheck=true;
    }
    else
    {
      motors.setSpeeds(-MOTOR_SPEED * 1.2, MOTOR_SPEED);
    }
    }
    rotatCheck = false;
  break;
  }
}

//Calibration function for white lines
void calibrWht() { //A funtion that is called in the setup to help calibrate sensors for the conditions at hand
    buttonA.waitForPress();
    lcd.print("Prs A Cal");
    delay(250);
    buttonA.waitForPress();
    readSensors(sensorState);
    /*
    threshold = ((sensorValues[0] + sensorValues[4]) / 2 + 20); //takes the mean value of far left and right sensors and adds some margin to create a threshold
    threshold2 = ((sensorValues[1] + sensorValues[3])/2 + 20);
    threshold3 = (sensorValues[2]+20); 
    */
    // Some manually set thresholds for easier testing
    threshold = 400; 
    threshold2 = 300;
    threshold3 = 300; 
    lcd.clear();
}

//Function that fills struct with values from sensorValues and compares to thresholds
void readSensors(LineSensorsWhite& state) {  // Next line reads the sensor values and store them in the array lineSensorValues
    lineSensors.read(sensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF); //Retrieves data from sensors
    state = {false,false,false,false,false}; // state of the sensors is ALWAYS set to negative in the structure, so that the if statements below only change the boolean to true when the conditions are met
    if (sensorValues[0] < threshold) {
        state.left = true;
    }
    if (sensorValues[1] < threshold2) {
        state.leftCenter = true;
    }
    if (sensorValues[2] < threshold3) {
        state.Center = true;
    }
    if (sensorValues[3] < threshold2) {
        state.rightCenter = true;
    }
    if (sensorValues[4] < threshold) {
        state.right = true;
    }
}

//Code for using gyro
void turnSensorSetup()
{
  Wire.begin();
  imu.init();
  imu.enableDefault();
  imu.configureForTurnSensing();
  lcd.clear();
  lcd.print(F("Gyro cal"));
  
  // Turn on the yellow LED in case the LCD is not available.
  ledYellow(1);
  // Calibrate the gyro.
  int32_t total = 0;
  for (uint16_t i = 0; i < 1024; i++)
  {
    // Wait for new data to be available, then read it.
    while(!imu.gyroDataReady()) {}
    imu.readGyro();
    // Add the Z axis reading to the total.
    total += imu.g.z;
  }
  ledYellow(0);
  gyroOffset = total / 1024;
  // Display the angle (in degrees from -180 to 180)
  lcd.clear();
}

void turnSensorReset()
{
  gyroLastUpdate = micros();
  turnAngle = 0;
}

// Updates imu sensor readings and converts angle to the interval [0;360]
void updateAngle()
{
  // Updates readings from gyro
  imu.readGyro();
  turnRate = imu.g.z - gyroOffset;
  uint16_t m = micros();
  uint16_t dt = m - gyroLastUpdate;
  gyroLastUpdate = m;
  int32_t d = (int32_t)turnRate * dt;
  turnAngle += (int64_t)d * 14680064 / 17578125; // We're sorry. Carlos pls explain <3
  // Converts angle interval from [-180;180] to [0;360]
  turnAngleDegrees=((((int32_t)turnAngle >> 16) * 360) >> 16);
  if(turnAngleDegrees <= 0  && turnAngleDegrees >= -180)
  {
  globalTheta = (360 + turnAngleDegrees);
  }
  if(turnAngleDegrees >= 0 && turnAngleDegrees <= 180)
  {
  globalTheta = turnAngleDegrees;
  }
  
  // For Debugging
  /*
  lcd.clear();
  lcd.gotoXY(0,1);
  lcd.print(globalTheta);
  */
}
