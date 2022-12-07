#include <DeadReckoner.h>
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4IMU imu;
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;

//DEAD RECKONER//////////////////////////////////////////////

// ENCODER PINS from Zumo32U4 documentation
#define ENCODER_LEFT_PIN 8
#define ENCODER_RIGHT_PIN 7

// Measurements
#define RADIUS 400 // wheel radius in mm
#define LENGTH 1000 // wheel base length in mm
#define TICKS_PER_REV 909.7 // From Zumo32U4 documentation: if 75:1 motors (which have gear ratios more accurately specified as 75.81:1) are used, the encoders provide 75.81 × 12 ≈ 909.7 CPR.(Counts per revolution)

#define MOTOR_SPEED 100

// Time Intervals
#define POSITION_COMPUTE_INTERVAL 50 // milliseconds
#define SEND_INTERVAL 100 // milliseconds

// Defines left and rightTicks storage values for encoders
volatile unsigned long leftTicks, rightTicks;

// Previous times for computing elapsed time.
unsigned long prevPositionComputeTime = 0, prevSendTime = 0;

// Previous x and y coordinate.
double prevX = 0, prevY = 0;

deadReckoner(&leftTicks, &rightTicks, TICKS_PER_REV, RADIUS, LENGTH);

void pulseLeft() { leftTicks++; }
void pulseRight() { rightTicks++; }

/**
Attaches interrupt and disables all serial communications.
This is necessary because when interrupts are active, incoming serial communication can be lost.
*/
void attachInterrupts() 
{
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_PIN), pulseLeft, HIGH);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_PIN), pulseRight, HIGH);
}

///////////////////////////////////////////////////////////

//Booleans that help the robot know how far in the process it is
bool rotatCheck = false;
////////////////////////Odometry Constants and variables/////////////////////////
int wheelDiameter = 4; //4 centimeters
double globalTheta, momentTheta, goalTheta;
//////////////////////////////////////////////////////
////////////////Gyro setup/////////////////////
int turnAngleDegrees, flippedturnAngleDegrees, turnAngle2;
uint32_t turnAngle = 0; 
uint16_t gyroLastUpdate = 0;
int16_t turnRate, gyroOffset;
//////////////////////////////////////////////


void setup() {
  attachInterrupts();
  Serial.begin(9600);
  gyroSetup(); //Sets up turning sensors
  delay(500);
  gyroReset();
  lcd.clear();
}

void loop() {
  if (millis() - prevPositionComputeTime > POSITION_COMPUTE_INTERVAL) {
    // Computes the new angular velocities and uses that to compute the new position.
    // The accuracy of the position estimate will increase with smaller time interval until a certain point.
    deadReckoner.computePosition();
    prevPositionComputeTime = millis();
  }

  if (millis() - prevSendTime > SEND_INTERVAL) {
    // Cartesian coordinate of latest location estimate.
    // Length unit correspond to the one specified under MEASUREMENTS.
    double x = deadReckoner.getX();
    double y = deadReckoner.getY();

    // Left and right angular velocities.
    double wl = deadReckoner.getWl();
    double wr = deadReckoner.getWr();

    // getTheta method returns the robot position angle in radians measured from the x axis to the center of the robot.
    // This angle is set initially at zero before the robot starts moving.
    double theta = deadReckoner.getTheta();

    // Total distance robot has troubled.
    double distance = sqrt(x * x + y * y);

    Serial.print("x: "); Serial.print(x);
    Serial.print("\ty: "); Serial.print(y);
    Serial.print("\twl: "); Serial.print(wl);
    Serial.print("\twr: "); Serial.print(wr);
    Serial.print("\ttheta: "); Serial.print(theta*RAD_TO_DEG); // theta converted to degrees.
    Serial.print("\tdist: "); Serial.println(distance);

    prevSendTime = millis();
  }

}

//Functions
void updateAngle()
{
  gyroUpdate();
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
  lcd.gotoXY(0,0);
  lcd.print(globalTheta);
  */
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

//Code for using gyro
void gyroSetup()
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

//Resets Gyro
void gyroReset()
{
  gyroLastUpdate = micros();
  turnAngle = 0;
}

//Updates gyro
void gyroUpdate()
{
  imu.readGyro();
  turnRate = imu.g.z - gyroOffset;
  uint16_t m = micros();
  uint16_t dt = m - gyroLastUpdate;
  gyroLastUpdate = m;
  int32_t d = (int32_t)turnRate * dt;
  turnAngle += (int64_t)d * 14680064 / 17578125; // We're sorry. Carlos pls explain <3
}
