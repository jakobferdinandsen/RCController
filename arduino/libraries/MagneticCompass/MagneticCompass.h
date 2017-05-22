/*
Magnetic Compass - Used to get the actual angel from north clokwise.
Created by Jonas Hansen May 13, 2017.
*/

#ifndef MagneticCompass_h
#define MagneticCompass_h

#include "Arduino.h"

class MagneticCompass
   {
    public:
      MagneticCompass(int pwmPin);
      int getHeading();

    private:
      int _pwmPin;
   };

#endif
