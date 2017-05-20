/* Include */
#include <MagneticCompass.h>      //Library that contains magnetic compass funktions
#include <Wire.h>                 //Library for comunication with magnetic compass
#include <DistanceSensor.h>       //Library that contains distance sensor funktions
#include <Servo.h>                //Library that contains servo funktions
#include <ArduinoJson.h>          //Library that contains json funktions

/* Drive mode setup*/
int driveMode = 4;                //Controls the different drive modes
int startDegrees = 0;             //Used to control when to turn in the different cases
int targetDegrees = 0;              //Used to define the next turn in the different cases
long resetTime = 0;
#define patternSpeed 140;
#define patternTurnValue 40;

int distanceForward = 0;
int distanceBackward = 0;

/* Servo setup */
Servo myServo;                    //create servo object to control a servo
int servoPosRight = 0;            //Servo position right
int servoPosLeft = 0;             //Servo position left
int servoPosInit = 89;            //Initialising servo position
int servoControl = 100;           //Servo control int 200-100=right 100-0=left
int servoControlBluetooth = 100;  //Servo control bluetooth same attributes as servoControl

/* H-bridge setup */
#define  IN_1  3                  //Defining IN_1 as pin 3, PWM signal for forward movement(bridge 1)
#define  IN_2  11                 //Defining IN_2 as pin 11, PWM signal for backward movement(bridge 2)
#define  INH_1 12                 //Defining INH_1 as pin 12, inhibit signal for bridge 1 
#define  INH_2 13                 //Defining INH_1 as pin 13, inhibit signal for bridge 2
long brakeDistanceBackward = 100; //Brake distance in CM
long brakeDistanceForward = 100;  //Brake distance in CM
int motorSpeedForward = 0;        //Motor speed forward PWM
int motorSpeedBackward = 0;       //Motor speed backward PWM
int motorControl = 100;           //Motor control int 200-100=forward speed 100-0=backward speed
int motorControlBluetooth = 100;  //Motor bluetooth control same attributes as motorControl
int motorControlIntern = 100;     //Motor intern control same attributes as motorControl
int pwmMax = 255;                 //Max PWM signal to DCmotor

/* Distance sensors setup*/
DistanceSensor forwardSensor(4, 2);   //Setup for forward sensor with trigPin 4, echoPin 2
DistanceSensor backwardSensor(8, 7);  //Setup for backward sensor with trigPin 8, echoPin 7

/* Mangnetic compass setup*/
MagneticCompass compassDegrees(0x60); //Setup for magnetic compass with address 0x60 (compass pin2 -> board analog5, compass pin3 -> board analog4)

void setup() {
  Wire.begin();                   //Starts comunication with magnetic compass
  Serial.begin(115200);           //Starts bluetooth serial comunication
  /* Servo */
  myServo.attach(9);              //attaches the servo on pin 9 to the servo object
  myServo.write(servoPosInit);    //tell servo to go to position, left = 110, middel = 89, right = 65
  /* H-Bridge */
  pinMode(IN_1, OUTPUT);          //H-brige pinmode for IN_1
  pinMode(IN_2, OUTPUT);          //H-brige pinmode for IN_2
  pinMode(INH_1, OUTPUT);         //H-brige pinmode for INH_1
  pinMode(INH_2, OUTPUT);         //H-brige pinmode for INH_2
  resetPorts();                   //H-brige reset input ports on bridge 1 and 2
  digitalWrite(INH_1, 1);         //H-brige sets sleep mode to off on bridge 1
  digitalWrite(INH_2, 1);         //H-brige sets sleep mode to off on bridge 2
}

void resetPorts() {
  /* Reset input on H-bridge 1 and 2 */
  digitalWrite(IN_1, 0);
  digitalWrite(IN_2, 0);
}

void turn(int target) {
  int current = compassDegrees.getDegrees();
  bool result = true;
  int diff = 0;
  if (target > current) {
    diff = target - current;
  } else {
    diff = current - target;
  }
  int dist = 0;
  if (diff > 180) {
    dist = 360 - diff;
  } else {
    dist = diff;
  }
  if (diff != dist) {
    result = current > 180;
  } else {
    result = target - current > 0;
  }

  if (result) {
    servoControl = 100 + patternTurnValue;
  } else {
    servoControl = 100 - patternTurnValue;
  }
}

void serialEvent() {
  /*Bluetooth */
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(Serial);
  if (json.success()) {
    motorControlBluetooth = json["speed"];
    servoControlBluetooth = json["direction"];
    driveMode = json["control"];
  }
}

void loop() {
  /*Drive mode*/
  switch (driveMode) {
    case 0:                                 //STOP!
      resetPorts();
      myServo.write(servoPosInit);
      break;
    case 1:                                 //Drive in square
      squareMode();
      break;
    case 2:                                 //Drive in rectangel
      rectangelMode();
      break;
    case 3:                                 //Drive in figure eight pattern
      eightMode();
      break;
    case 4:                                 //Drive in manuel mode
      manuelMode();
      break;
  }
  run();
  delay(1);
}

void squareMode() {
      motorControl = patternSpeed;

      if (resetTime == 0) {
        resetTime = millis();
      }
      if (millis() - resetTime > 2500) {
        resetTime = 0;
        startDegrees = 0;
        targetDegrees = startDegrees + 90;
        if (targetDegrees > 359) {
          targetDegrees -= 360;
        }
      }
      turn(targetDegrees);
}

  void rectangelMode() {
    distanceForward = forwardSensor.getDistance();
    if (distanceForward > 50) {
      analogWrite(IN_2, 50);
    } else {
      resetPorts();
    }
  }

  void eightMode() {




  }

void manualMode(){
      servoControl = servoControlBluetooth;
      motorControl = motorControlBluetooth;
}

  void run() {
    /* Servo control*/
    if (servoControl < 100) {
      servoPosRight = map(servoControl, 101, 200, 89, 65);    //Maps int from 100-200 to 89-65
      myServo.write(servoPosRight);                         //Writes mapped pos to servo
    } else if (servoControl > 100) {
      servoPosLeft = map(servoControl, 99, 0, 89, 113);  //Maps int from 100-0 to 89-113
      myServo.write(servoPosLeft);                          //Writes mapped pos to servo
    } else {
      myServo.write(servoPosInit);                          //Writes initial pos to servo
    }

    /* H-bridge/DCmotor control*/
    /*Backward*/
    motorSpeedBackward = map(motorControl, 100, 0, 0, pwmMax);  //Maps int motorControl from 100-0 to 0-pwmMax(0-255)
    //brakeDistanceBackward = (motorSpeedBackward * 10) / 10 + 8; //Brake distance in CM

    /*Forward*/
    motorSpeedForward = map(motorControl, 100, 200, 0, pwmMax); //Maps int motorControl from 100-200 to 0-pwmMax(0-255)
    //brakeDistanceForward = (motorSpeedForward * 10) / 10 + 8;   //Brake distance in CM


    /*Speed control*/
    if (motorControl < 100) {        // && backwardSensor.getDistance() > brakeDistanceBackward
      analogWrite(IN_1, motorSpeedBackward);  //Writes mapped speed to DCmotor
    } else if (motorControl > 100) { // && forwardSensor.getDistance() > brakeDistanceForward
      analogWrite(IN_2, motorSpeedForward); //Writes mapped speed to DCmotor
    } else {
      resetPorts();
    }
  }

