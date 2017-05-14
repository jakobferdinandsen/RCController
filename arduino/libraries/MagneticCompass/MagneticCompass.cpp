/*

*/

#include "Arduino.h"
#include "Wire.h"
#include "MagneticCompass.h"

MagneticCompass::MagneticCompass(int address)
{
	_address = address;
}

int MagneticCompass::getDegrees()
{
      byte highByte;
      byte lowByte;

      Wire.beginTransmission(_address);      //starts communication with cmps03
      Wire.write(2);                        //Sends the register we wish to read
      Wire.endTransmission();

      Wire.requestFrom(_address, 2);        //requests high byte
      while (Wire.available() < 2);        //while there is a byte to receive
      highByte = Wire.read();              //reads the byte as an integer
      lowByte = Wire.read();
      int bearing = ((highByte << 8) + lowByte) / 10;
      return bearing;
    }
