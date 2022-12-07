#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include <math.h>

#define MOTOR_SPEED 120

Zumo32U4LineSensors lineSensors;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;
Zumo32U4IMU imu;
Zumo32U4LCD lcd;

bool rotatCheck, reachedDest, finished, traveling = false;
bool sensorreads = true;
bool bounce = true;
bool reset = true;

////////////////////////Odometry Constants and variables/////////////////////////
int wheelDiameter = 4; //4 centimeters
int deltaT = 25;
double currentX, currentY, deltax, deltay = 0;
int globalTheta;
double momentTheta, goalTheta, deltaD;
double velocityR, velocityL, fwdVelocity = 0;

int drivenDist;
int angle;

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
LineSensorsWhite sensorState = {false,false,false,false,false};

////////////////Gyro setup/////////////////////
int turnAngleDegrees;
uint32_t turnAngle = 0;
uint16_t gyroLastUpdate = 0;
int16_t turnRate, gyroOffset;


struct Waypoint
{
    int X;
    int Y;
    Waypoint* next;

    Waypoint(int x, int y) {
        X = x;
        Y = y;
        next = NULL;
    }
};

void debugLog(String text) {
    Serial.println("[DEBUG]<" + (String)__TIME__ + ">: " + text);
}

struct WaypointQueue
{
    Waypoint* front, * rear;

    WaypointQueue() {
        front = rear = NULL;
    }

    void enQueue(int x, int y)
    {
        //debugLog("Enqueuing new waypoint.");
        Waypoint* temp = new Waypoint(x, y);

        if (rear == NULL) {
            front = rear = temp;
            return;
        }

        rear->next = temp;
        rear = temp;
        debugLog("Waypoint has been enqueued. Data (X): " + (String)temp->X + ", (Y): " + (String)temp->Y);
    }

    void deQueue()
    {
        if (front == NULL) {
            return;
        }

        Waypoint* temp = front;
        front = front->next;

        if (front == NULL) {
            rear = NULL;
        }

        //debugLog("Removed waypoint from queue. Data (X): " + (String)temp->X + ", (Y): " + (String)temp->Y);
        delete(temp);
        //debugLog("Removed. X value for the next waypoint in queue: " + (String)front->X)
    }
};

WaypointQueue waypointQueue;

void setup() {
    configureComponents();
}

void loop() {
      waypointQueue.deQueue();
      if (waypointQueue.front != NULL) {
        boundaryBounce();
        goTo(waypointQueue.front->X, waypointQueue.front->Y);
    }
    else {
        finished = true;
    }
    if(finished && reset){
      reset = false;
      turn(0,'R');
      motors.setSpeeds(-100,-100);
      delay(500);
      motors.setSpeeds(0,0);
    }
}

void boundaryBounce()
{
  oldTurn(45,'L');
  motors.setSpeeds(100,100);
  while(bounce){
    readSensors(sensorState);
    updatePosition();
    long timeTaken = millis();
    if(sensorState.leftCenter)
    {
      oldTurn(100,'R');
      motors.setSpeeds(100,100);
    }
    if(sensorState.rightCenter)
    {
      oldTurn(100,'L');
      motors.setSpeeds(100,100);
    }
    if(timeTaken > 60000 )
    {
      bounce = false;
    }
  }
}

void oldTurn(int desiredAngle, char Direction)
{
  switch(Direction)
  {
    case 'R':
    updatePosition();
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
    updatePosition();
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
    updatePosition();
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
    updatePosition();
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

void gyroscopeSetup() {
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
    for (uint16_t i = 0; i < 1024; i++) {
        // Wait for new data to be available, then read it.
        while (!imu.gyroDataReady()) {}
        imu.readGyro();
        // Add the Z axis reading to the total.
        total += imu.g.z;
    }
    ledYellow(0);
    gyroOffset = total / 1024;
    delay(250);
    // Display the angle (in degrees from -180 to 180)
    lcd.clear();
}

void gyroscopeReset() {
    gyroLastUpdate = micros();
    turnAngle = 0;
}

void turn(int goalAngle, char Direction)
{
    switch (Direction)
    {
    case 'R':
        updateAngle();

        while (!rotatCheck) {
            updatePosition();
            if (globalTheta >= goalAngle && (goalAngle) >= globalTheta) {
                motors.setSpeeds(0, 0);
                rotatCheck = true;
            }
            else {
                motors.setSpeeds(100,-85);
            }
        }
        rotatCheck = false;
        break;

    case 'L':
        updateAngle();
        while (!rotatCheck) {
            updatePosition();
            if (globalTheta >= goalAngle && (goalAngle) >= globalTheta) {
                motors.setSpeeds(0, 0);
                rotatCheck = true;
            }
            else {
                motors.setSpeeds(-100,100);
            }
        }
        rotatCheck = false;
        break;
    }
}

bool verifyDestination(bool negX, bool negY, int x, int y, bool inverseX, bool inverseY)
{
    // TODO: Make sure it compensates for currentX already bigger than x, same goes for currentY and y.
    bool ret = false;
    int xEqL = inverseX ? x : currentX;
    int yEqL = inverseY ? y : currentY;
    int xEqR = inverseX ? currentX : x;
    int yEqR = inverseY ? currentY : y;

    if (negX && negY) {
        debugLog("Moving to a pos where: " + (String)yEqL + " <= " + (String)yEqR);
        ret = xEqL <= xEqR && yEqL <= yEqR ? true : false;
    }
    else if (!negX && !negY) {
        debugLog("Moving to a pos where: " + (String)yEqL + " >= " + (String)yEqR);
        ret = xEqL >= xEqR && yEqL >= yEqR ? true : false;
    }
    else if (!negX && negY) {
        debugLog("Moving to a pos where: " + (String)yEqL + " <= " + (String)yEqR);
        ret = xEqL >= xEqR && yEqL <= yEqR ? true : false;
    }
    else if (negX && !negY) {
        debugLog("Moving to a pos where: " + (String)yEqL + " >= " + (String)yEqR);
        ret = xEqL <= xEqR && yEqL >= yEqR ? true : false;
    }
    //debugLog("negX: " + (String)negX + ", negY: " + (String)negY);
    return ret;
}

void goTo(int x, int y) {
    //debugLog("Moving to position X: " + (String)x + ", Y: " + (String)y);
    traveling = true;
    int destx = x - currentX;
    int desty = y - currentY;
    int angleDeg = atan2(desty, destx) * 180 / PI;
    angle = (angleDeg + 360) % 360;
    char dir;

    if(globalTheta > 245 && angle < 90)
    {
    dir = 'L';
    }
    else
    {
    dir = 0 < angle && angle < 180 ? 'L' : 'R';
    }
    
    //bool neg = angle < 0 ? true : false;
    bool destXNeg = x < 0 ? true : false;
    bool destYNeg = y < 0 ? true : false;
    reachedDest = false;
    turn(angle, dir);
    moveForward(x, y, destXNeg, destYNeg, angle);
}

void moveForward(int x, int y, bool destXneg, bool destYneg, int angle)
{
    debugLog("Moving to x: " + (String)x + ", y: " + (String)y);
    motors.setSpeeds(MOTOR_SPEED, MOTOR_SPEED);
    bool xInverse = currentX > (x - 5) && destXneg || currentX > (x + 5) && !destXneg ? true : false;
    bool yInverse = currentY > (y - 5) && destYneg || currentY > (y + 5) && !destYneg ? true : false;
    debugLog("X inverse: " + (String)xInverse + ", Y inverse: " + (String)yInverse);
    while (!verifyDestination(destXneg, destYneg, x, y, xInverse, yInverse) && !reachedDest)
    {
        if (0 < encoders.getCountsLeft() || 0 < encoders.getCountsRight()) {            
            updatePosition();
        }
    }
    motors.setSpeeds(0, 0);
    reachedDest = true;
    traveling = false;
    debugLog("Done! Current pos x: " + (String)currentX + ", y: " + (String)currentY);
}

void updateAngle()
{
    // Updates readings from gyro
    imu.readGyro();
    turnRate = imu.g.z - gyroOffset;
    uint16_t m = micros();
    uint16_t dt = m - gyroLastUpdate;
    gyroLastUpdate = m;
    int32_t d = (int32_t)turnRate * dt;
    turnAngle += (int64_t)d * 14680064 / 17578125;
    
    // Converts angle interval from [-180;180] to [0;360]
    turnAngleDegrees = ((((int32_t)turnAngle >> 16) * 360) >> 16);
    if (turnAngleDegrees <= 0 && turnAngleDegrees >= -180) {
        globalTheta = (360 + turnAngleDegrees);
    }
    if (turnAngleDegrees >= 0 && turnAngleDegrees <= 180) {
        globalTheta = turnAngleDegrees;
    }
}

void updatePosition()
{
    updateAngle();
    float encL = encoders.getCountsLeft();
    float encR = encoders.getCountsRight();
    fwdVelocity = (((encL + encR) / 2) / 909.7 * (PI * wheelDiameter));
    velocityR = ((encR) / 909.7 * (PI * wheelDiameter));
    velocityL = ((encL) / 909.7 * (PI * wheelDiameter));
    deltaD = (velocityR + velocityL) / 2;
    encoders.getCountsAndResetLeft();
    encoders.getCountsAndResetRight();
    deltax = (deltaD * cos(globalTheta * (PI / 180)));
    deltay = (deltaD * sin(globalTheta * (PI / 180)));

    currentX = currentX + deltax;
    currentY = currentY + deltay;

    lcd.clear();
    lcd.gotoXY(0, 0);
    lcd.print("x: " + (String)currentX);
    lcd.gotoXY(0, 1);
    lcd.print("y: " + (String)currentY);
}

void calibrWht() { //A funtion that is called in the setup to help calibrate sensors for the conditions at hand
    buttonA.waitForPress();
    Serial.println("Press A to calibrate WHITE");
    delay(250);
    buttonA.waitForPress();
    threshold = ((sensorValues[1] + sensorValues[3]) / 2 + 20); //takes the mean value of far left and right sensors and adds some margin to create a threshold
    delay(250);
    Serial.println(threshold); //prints threshold once at the beginning of the code
}

void readSensors(LineSensorsWhite& state) {  // Next line reads the sensor values and store them in the array lineSensorValues
    lineSensors.read(sensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF); //Retrieves data from sensors
    state = {false,false,false,false,false}; // state of the sensors is ALWAYS set to negative in the structure, so that the if statements below only change the boolean to true when the conditions are met
    if (sensorValues[0] < threshold) {
        state.left = true;
    }
    if (sensorValues[1] < threshold) {
        state.leftCenter = true;
    }
    if (sensorValues[2] < threshold) {
        state.Center = true;
    }
    if (sensorValues[3] < threshold) {
        state.rightCenter = true;
    }
    if (sensorValues[4] < threshold) {
        state.right = true;
    }
}
void configureComponents() {
    Serial.begin(9600);
    gyroscopeSetup();
    delay(500);
    gyroscopeReset();
    lcd.clear();
    buttonA.waitForPress();
    delay(250);
    lineSensors.initFiveSensors(); // Initializes five sensors
    readSensors(sensorState);      //fills structure with first bools
    calibrWht();
    waypointQueue.enQueue(0, 20);
    waypointQueue.enQueue(0, 0);
    goTo(waypointQueue.front->X, waypointQueue.front->Y);
}
