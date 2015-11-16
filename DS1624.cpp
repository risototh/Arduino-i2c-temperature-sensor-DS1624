/*
  Library for controlling the DS1624 ic with Arduino
  Tested with arduino-0018
  26/03/2010 Release 0.1
  Written by Federico and Riccardo Galli
  http://www.sideralis.org
  fixed by Richard Toth http://risko.org
*/

#include "DS1624.h"
#include <Wire.h>
#include <Arduino.h>

DS1624::DS1624(int8_t addr)
{
  this->addr=addr;
  Wire.begin();
}

void DS1624::start()
{
    Wire.beginTransmission(this->addr);
    Wire.write(0xAC);
    Wire.write(CONTINUOUS_CONVERSION); //Put the DS1624 in continuos conversion mode
    Wire.endTransmission();
    delay (100); //Min time needed to store the previous command is 10ms
    Wire.beginTransmission(this->addr);
    Wire.write(CONVERT_T); //Enable the continuos conversion mode
    Wire.endTransmission();
}

float DS1624::getTemp()
{
  uint16_t t = 0;
  
  Wire.beginTransmission(this->addr);
  Wire.write(READ_T);
  Wire.requestFrom(this->addr, 2);

  if (Wire.available()) t = Wire.read() << 8;
  if (Wire.available()) t |= Wire.read();
  
  Wire.endTransmission();

  t >>= 4;
  return (float)((t & 0x800 ? (t & 0x7ff) - 0x800 : t) / 16.0);
}
