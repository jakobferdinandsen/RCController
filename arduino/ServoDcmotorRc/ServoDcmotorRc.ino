class DistanceSensor {
  private:

    int trigPin;
    int echoPin;

  public:

    DistanceSensor(int trigPin, int echoPin) {
      this->trigPin = trigPin;
      this->echoPin = echoPin;
    }

    int getDistance() {
      /* establish variables for duration of the ping,
         and the distance result in inches and centimeters: */
      long duration, cm;

      /* The sensor is triggered by a HIGH pulse of 10 or more microseconds.
         Give a short LOW pulse beforehand to ensure a clean HIGH pulse: */
      pinMode(trigPin, OUTPUT);
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      /* Read the signal from the sensor: a HIGH pulse whose
         duration is the time (in microseconds) from the sending
         of the ping to the reception of its echo off of an object.*/
      pinMode(echoPin, INPUT);
      duration = pulseIn(echoPin, HIGH);

      /* convert the time into a distance */
      cm = microsecondsToCentimeters(duration);
      return cm;
    }

    long microsecondsToCentimeters(long microseconds)
    {
      /* The speed of sound is 340 m/s or 29 microseconds per centimeter.
         The ping travels out and back, so to find the distance of the
         object we take half of the distance travelled. */
      return microseconds / 29 / 2;
    }
};

#include <Wire.h>
class MagneticCompass {
  private:

    int address;

  public:

    MagneticCompass(int address) {
      this->address = address;
    }
    int getDegrees() {
      byte highByte;
      byte lowByte;

      Wire.beginTransmission(address);      //starts communication with cmps03
      Wire.write(2);                        //Sends the register we wish to read
      Wire.endTransmission();

      Wire.requestFrom(address, 2);        //requests high byte
      while (Wire.available() < 2);        //while there is a byte to receive
      highByte = Wire.read();              //reads the byte as an integer
      lowByte = Wire.read();
      int bearing = ((highByte << 8) + lowByte) / 10;
      return bearing;
    }
};


/* Servo setup */
#include <Servo.h>
Servo myServo;              //create servo object to control a servo
int servoPosRight = 0;      //Servo position right
int servoPosLeft = 0;       //Servo position left
int servoPosInit = 89;      //Initialising servo position
int servoPosBlue = 100;     //Bluetooth int 200-100=Left 100-0=Right

/* H-bridge setup */
#define  IS_1  0
#define  IS_2  1
#define  IN_1  3
#define  IN_2  11
#define  INH_1 12
#define  INH_2 13

int motorDCForward = 0;     //actual DC forward
int motorDCBackward = 0;    //actual DC backward
int motorBlue = 100;        //Bluetooth int 200-100=forward speed 100-0=backward speed
int pwmMax = 100;           //Max pwm signal to DCmotor

/* Bluetooth */
#include <ArduinoJson.h>
int speed = 100;
int direction = 100;

/* Sensors */
DistanceSensor forwardSensor(4, 2);
DistanceSensor backwardSensor(8, 7);

/* Mangnetic Compass*/
MangneticCompass compassDegrees(0x60);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     //Starts bluetooth serial komunication
  /* Servo */
  myServo.attach(9);          //attaches the servo on pin 9 to the servo object
  myServo.write(servoPosInit);//tell servo to go to position, venstre = 110, midt = 89, højre = 65
  /* H-Bridge */
  pinMode(IN_1, OUTPUT);      //H-brige pinmode for IN_1
  pinMode(IN_2, OUTPUT);      //H-brige pinmode for IN_2
  pinMode(INH_1, OUTPUT);     //H-brige pinmode for INH_1
  pinMode(INH_2, OUTPUT);     //H-brige pinmode for INH_2
  resetPorts();               //H-brige reset input ports on bridge 1 and 2
  digitalWrite(INH_1, 1);     //H-brige sets sleep mode to off on bridge 1
  digitalWrite(INH_2, 1);     //H-brige sets sleep mode to off on bridge 2
}

// Reset INPUT on Bridge 1 and 2
void resetPorts()
{
  digitalWrite(IN_1, 0);
  digitalWrite(IN_2, 0);
}

void loop() {
  /* Bluetooth */
  StaticJsonBuffer<200> jsonBuffer;
  String t;                                      //string to hold data from BT module
  while (Serial.available()) {                   //keep reading bytes while they are still more in the buffer
    t += (char)Serial.read();                    //read byte, convert to char, and append it to string
  }

  if (t.length()) {                              //if string is not empty do the following
    char data[200];
    t.toCharArray(data, 200);
    JsonObject& json = jsonBuffer.parseObject(data);
    if (json.success()) {
      motorBlue = json["speed"];
      servoPosBlue = json["direction"];
    }
  }
  delay(5);

  /* Servo */
  if (servoPosBlue < 100) {
    servoPosRight = map(servoPosBlue, 100, 0, 89, 65);    //Maps int from 100-0 to 89-65
    myServo.write(servoPosRight);                         //Writes mapped pos to servo
  } else if (servoPosBlue > 100) {
    servoPosLeft = map(servoPosBlue, 100, 200, 89, 113);  //Maps int from 100-200 to 89-113
    myServo.write(servoPosLeft);                          //Writes mapped pos to servo
  } else {
    myServo.write(servoPosInit);                          //Writes initial pos to servo
  }
  /* H-bridge */
  if (motorBlue < 100 && backwardSensor.getDistance() > 20) {
    motorDCBackward = map(motorBlue, 100, 0, 0, pwmMax);  //Maps int from 100-0 to 0-pwmMax(0-255)
    analogWrite(IN_1, motorDCBackward);                   //Writes mapped speed to DCmotor
  } else if (motorBlue > 100 && forwardSensor.getDistance() > 20) {
    motorDCForward = map(motorBlue, 100, 200, 0, pwmMax); //Maps int from 100-200 to 0-pwmMax(0-255)
    analogWrite(IN_2, motorDCForward);                    //Writes mapped speed to DCmotor
  } else {
    resetPorts();
  }
}
