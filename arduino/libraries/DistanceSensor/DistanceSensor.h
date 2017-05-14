/*
  DistanceSensor.h - For use with ultrasound sensores


*/
#ifndef DistanceSensor_h
#define DistanceSensor_h

#include "Arduino.h"

class DistanceSensor
 {
  public:
    DistanceSensor(int trigPin, int echoPin);
    int getDistance();

  private:
    int _trigPin;
    int _echoPin;

    };

#endif

