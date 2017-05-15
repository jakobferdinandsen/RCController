/*
DistanceSensor
*/
#include "Arduino.h"
#include "DistanceSensor.h"

DistanceSensor::DistanceSensor(int trigPin, int echoPin)
{
	_trigPin = trigPin;
	_echoPin = echoPin;
}


int DistanceSensor::getDistance()
  {
      /* establish variables for duration of the ping,
         and the distance result in inches and centimeters: */
      long duration, cm;

      /* The sensor is triggered by a HIGH pulse of 10 or more microseconds.
         Give a short LOW pulse beforehand to ensure a clean HIGH pulse: */
      pinMode(_trigPin, OUTPUT);
      digitalWrite(_trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(_trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(_trigPin, LOW);

      /* Read the signal from the sensor: a HIGH pulse whose
         duration is the time (in microseconds) from the sending
         of the ping to the reception of its echo off of an object.*/
      pinMode(_echoPin, INPUT);
      duration = pulseIn(_echoPin, HIGH);

      /* convert the time into a distance */
      cm = duration / 29 / 2;
      /* The speed of sound is 340 m/s or 29 microseconds per centimeter.
         The ping travels out and back, so to find the distance of the
         object we take half of the distance travelled. */
      return cm;
    }
      
