/*

*/

#include "Arduino.h"
#include "Wire.h"
#include "MagneticCompass.h"

MagneticCompass::MagneticCompass(int pwmPin)
{
	_pwmPin = pwmPin;
}

int MagneticCompass::getHeading()
{
  pinMode(_pwmPin, INPUT);
	int pwmCal = map(pulseIn(_pwmPin, HIGH), 1000, 37000, 0, 359);
	return(pwmCal);
}
