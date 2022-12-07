#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include <math.h>

#define MOTOR_SPEED 120

Zumo32U4LCD lcd;
Zumo32U4IMU imu;
Zumo32U4ButtonA buttonA;
//Booleans that help the robot know how far in the process it is
bool rotatCheck, reachedDest, traveling = false;
bool sensorreads, bounce = true;

////////////////////////Odometry Constants and variables/////////////////////////
int wheelDiameter = 4; //4 centimeters
int axleLength = 9;
int deltaT = 25;
double currentX, currentY, deltax, deltay = 0;
int globalTheta;
double momentTheta, goalTheta, deltaD;
double velocityR, velocityL, fwdVelocity = 0;

//////////////////////// MOTOR ////////////////////////
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;
int drivenDist;
int angle;
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
}

void configureComponents() {
    Serial.begin(9600);
    gyroscopeSetup();
    delay(500);
    gyroscopeReset();
    lcd.clear();
    buttonA.waitForPress();
    delay(250);
    waypointQueue.enQueue(0, 35);
    waypointQueue.enQueue(8, 35);
    waypointQueue.enQueue(8, 10);
    waypointQueue.enQueue(16, 10);
    waypointQueue.enQueue(16, 35);
    waypointQueue.enQueue(24, 35);
    waypointQueue.enQueue(24, 10);
    waypointQueue.enQueue(32, 10);
    waypointQueue.enQueue(32, 35);
    waypointQueue.enQueue(40, 35);
    waypointQueue.enQueue(40, 0);
    waypointQueue.enQueue(0, 0);
    goTo(waypointQueue.front->X, waypointQueue.front->Y);
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
                motors.setSpeeds(-90,100);
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
    waypointQueue.deQueue();
    debugLog("Done! Current pos x: " + (String)currentX + ", y: " + (String)currentY);
    //debugLog("Moved to target position. Current position X: " + (String)currentX + ", Y: " + (String)currentY);

    if (waypointQueue.front != NULL) {
        goTo(waypointQueue.front->X, waypointQueue.front->Y);
    }
    else {
        return;
    }
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
    turnAngle += (int64_t)d * 14680064 / 17578125; // We're sorry. Carlos pls explain <3
    
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
  
    velocityR = ((encR) / 909.7 * (PI * wheelDiameter));
    velocityL = ((encL) / 909.7 * (PI * wheelDiameter));
    deltaD = (velocityR + velocityL) / 2;
    encoders.getCountsAndResetLeft();
    encoders.getCountsAndResetRight();
    deltax = (deltaD * cos(globalTheta * (PI / 180)));
    deltay = (deltaD * sin(globalTheta * (PI / 180)));

    currentX = currentX + deltax;
    currentY = currentY + deltay;

    //lcd.clear();
    //lcd.print("ang: " + (String)globalTheta);
    
    lcd.clear();
    lcd.gotoXY(0, 0);
    lcd.print("x: " + (String)currentX);
    lcd.gotoXY(0, 1);
    lcd.print("y: " + (String)currentY);
    //debugLog("Position has been updated.");
}
