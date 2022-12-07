#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4IMU imu;

//Booleans that help the robot know how far in the process it is
bool rotatCheck, firstDetection, secondDetection = false;
bool sensorreads, bounce = true;
////////////////////////World Constants/////////////////////////
int wheelRadius = 2; //2 centimeters
int axleLength = 9; //10 centimeters from one wheel to the other
long x, y, deltax, deltay, oldx, oldy = 0;
int deltaT = 100;
double globalTheta, localTheta, momentTheta, goalTheta;
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
bool useEmitters = true;
uint16_t brightnessLevels[]={1,1,3,3,5,5};

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

void setup() {
    //Calibrates various components and awaits a buttonpress to start
    configureComponents();
    delay(250);
    buttonA.waitForPress();
    delay(250);
}

void loop() 
{
  //turnL(90);
  //delay(1000);
 //boundaryBounce();

 moveForward(100,20);
 turn(90,'L');
 moveForward(100,20);
 turn(90,'L');
 moveForward(100,20);
 turn(90,'L');
 moveForward(100,20);
 turn(90,'L');

 moveForward(100,20);
 turn(90,'R');
 moveForward(100,20);
 turn(90,'R');
 moveForward(100,20);
 turn(90,'R');
 moveForward(100,20);
 turn(90,'R');

 delay(1000);
}

/////////////////////// FUNCTIONS ///////////////////////
void getAngle()
{
  turnSensorUpdate();
  turnAngleDegrees=((((int32_t)turnAngle >> 16) * 360) >> 16);

  if(turnAngleDegrees <= 0  && turnAngleDegrees >= -180)
  {
  globalTheta = (360 + turnAngleDegrees);
  }
  if(turnAngleDegrees >= 0 && turnAngleDegrees <= 180)
  {
  globalTheta = turnAngleDegrees;
  }
  lcd.clear();
  lcd.gotoXY(0,0);
  lcd.print(globalTheta);
}

void getPosition()
{
  getVelocity();
  getAngle();
  double R,W;
  if(velocityR == velocityL)
  {
  R = (axleLength/2) * (velocityR + velocityL)/((velocityR+0.1)-velocityL);
  W = (velocityR - velocityL)/axleLength;
  }
  else
  {
  R = (axleLength/2) * (velocityR + velocityL)/(velocityR-velocityL);
  W = (velocityR - velocityL)/axleLength;  
  }
  deltax = (cos(globalTheta*0.1)*R*sin(globalTheta))-(sin(W*0.1)*(-R*cos(globalTheta)))+R*sin(globalTheta);
  deltay = (sin(globalTheta*0.1)*R*sin(globalTheta))+(cos(W*0.1)*(-R*cos(globalTheta)))+R*cos(globalTheta);
  x = x + deltax;
  y = y + deltay;
  
 //For Debugging:
 /*
  Serial.println("////////////////////////////////////////////////");
  Serial.println("Rv: " + (String)velocityR + " Lv: " + (String)velocityL);
  Serial.println("R: " + (String)R + "  W: " + (String)W);
  Serial.println("Velocity: " + (String)fwdVelocity + " cm/s");
  Serial.println("Theta:    " + (String)globalTheta);
  Serial.println("x:        " + (String)x);
  Serial.println("y:        " + (String)y);
  Serial.println("////////////////////////////////////////////////");
 
  lcd.clear();
  lcd.gotoXY(0,0);
  lcd.print("x: " + (String)x);
  lcd.gotoXY(0,1);
  lcd.print("y: " + (String)y);
  */
}

//Calculates the velocities of the right and left belts as cm/s
void getVelocity()
{
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
  delay(deltaT);
  float encL = encoders.getCountsLeft();
  float encR = encoders.getCountsRight();
  //TODO Check velocity calculations for deltaT
  fwdVelocity = (((encL + encR)/2)/900 * (PI * 4))/(deltaT*0.001);
  velocityR = ((encR)/900 * (PI * 4))/(deltaT*0.001);
  velocityL = ((encL)/900 * (PI * 4))/(deltaT*0.001);
  
}

void boundaryBounce()
{
  while(bounce)
  {
    turnSensorUpdate();
    turnAngleDegrees=((((int32_t)turnAngle >> 16) * 360) >> 16); //Updates turnAngleDegrees
    motors.setSpeeds(MOTOR_SPEED,MOTOR_SPEED);
    readSensors(sensorState);
    getPosition();
    if(sensorState.left)
    {
    motors.setSpeeds(0,0);
    turn(100,'R'); 
    }
     
    if (sensorState.right) 
    {
    motors.setSpeeds(0,0);
    turn(100,'L');
    }
 }
}

void measureDist1()
{
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
  motors.setSpeeds(75,75);
while(!secondDetection)
  {
  //Reads and converts encoder counts to centimeters
  float LeftCount = encoders.getCountsLeft();
  float RightCount = encoders.getCountsRight();
  float AvgCount = (LeftCount + RightCount) / 2;
  double Distance = (AvgCount / 900) * (PI * 4);
  drivenDist = Distance;

  //Reads sensors and looks if right side is true
  readSensors(sensorState);
    if(sensorState.right)
    {
    Serial.println("Found second Line");
    motors.setSpeeds(0,0);
    lcd.clear();
    lcd.gotoXY(0,0);
    lcd.print((String)drivenDist);
    secondDetection = true;
    }
    delay(50);
  }
secondDetection = false;
drivenDist = 0;
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
    calibrWht();
    Serial.println("Press A to start");
}


//Reuseable function for driving distances in cm
void moveForward(int speeds,int dist){                //Functions for moving directions, expecting three values. One for speed and one for time
            while(drivenDist<dist){              //while desired distance travelled is greater than the actually travelled distance
            float countsL = encoders.getCountsLeft();               //Retrieve motorcounts
            double movementIncm = (countsL / 900) * (PI * 4) - 1.5; //Convert motorcounts to CM            
            motors.setSpeeds(speeds,speeds);                        //Set positive speeds to go forward
            drivenDist = movementIncm;
            }
            motors.setSpeeds(0,0);                                  //turn off motors
            encoders.getCountsAndResetLeft();                       //Reset encodercount for future use
            drivenDist = 0;
}

void turn(int desiredAngle, char Direction)
{
  switch(Direction)
  {
    case 'R':
    getPosition();
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
    getAngle();
    
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
    getPosition();
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
    getAngle();
    
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
    threshold = 400; //takes the mean value of far left and right sensors and adds some margin to create a threshold
    threshold2 = 300;
    threshold3 = 300; 
    
    delay(250);
    Serial.println(threshold); //prints threshold once at the beginning of the code
    Serial.println(threshold2);
    Serial.println(threshold3);
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

  // Delay to give the user time to remove their finger.
  delay(500);

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

  // Display the angle (in degrees from -180 to 180) until the
  // user presses A.
  lcd.clear();
}


void turnSensorReset()
{
  gyroLastUpdate = micros();
  turnAngle = 0;
}

void turnSensorUpdate()
{
  imu.readGyro();
  turnRate = imu.g.z - gyroOffset;
  uint16_t m = micros();
  uint16_t dt = m - gyroLastUpdate;
  gyroLastUpdate = m;
  int32_t d = (int32_t)turnRate * dt;
  turnAngle += (int64_t)d * 14680064 / 17578125; // We're sorry.
}
