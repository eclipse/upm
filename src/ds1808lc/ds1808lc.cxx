#include <iostream>
#include <unistd.h>
#include <string.h>
#include <cmath>
#include "ds1808lc.hpp"
#include "mraa-utils.hpp"

#define DS1808_I2C_ADDR 0x28
#define DS1808_POT2_OFFSET 0x40
#define DS1808_MAX_POT_VALUE 0x20
#define DS1808_LOW_VALUE 32 // Lowest pot value that the eye can differentiate from 0
#define DS1808_HIGH_VALUE 51 // Highest pot value that the eye can differentiate from full


namespace upm {
DS1808LC::DS1808LC(int gpioPower, int i2cBus)
{
   mraa_set_log_level(7);
   pinPower = gpioPower;
   i2c = new mraa::I2c(i2cBus);
   status = i2c->address(DS1808_I2C_ADDR);
   getBrightness();
}

DS1808LC::~DS1808LC()
{
}


bool DS1808LC::isPowered()
{
   return static_cast<bool>(MraaUtils::getGpio(pinPower));
}

void DS1808LC::setPowerOn()
{
   if (!isPowered())
   {
      MraaUtils::setGpio(pinPower, 1);
      setBrightness(0);
   }
}

void DS1808LC::setPowerOff()
{
   MraaUtils::setGpio(pinPower, 0);
}


int DS1808LC::getBrightness()
{
   uint8_t values[2];

   if (i2c->read(values, 2) == 2) {
      return getPercentBrightness(values[0], values[1]);
   }
   else
      UPM_THROW("i2c read error");
}


void DS1808LC::setBrightness(int dutyPercent)
{
   uint8_t values[2];
   values[0] = getPot1Value(dutyPercent);
   values[1] = getPot2Value(dutyPercent);
   status = i2c->write(values, 2);
   if (status != mraa::SUCCESS)
       UPM_THROW("i2c write error");
}

//
// Helper Function
//

uint8_t DS1808LC::getPot1Value(int dutyPercent) {
   uint8_t result = 0;
   int scaledResistance = getScaledResistance(dutyPercent);

   if (scaledResistance > DS1808_MAX_POT_VALUE)
      result = DS1808_MAX_POT_VALUE;
   else {
      result = scaledResistance;
   }

   return result;
}

uint8_t DS1808LC::getPot2Value(int dutyPercent) {
   uint8_t result = 0;
   int scaledResistance = getScaledResistance(dutyPercent);

   if (scaledResistance <= DS1808_MAX_POT_VALUE)
      result = 0;
   else {
      result = scaledResistance - DS1808_MAX_POT_VALUE;
   }

   if (result > DS1808_MAX_POT_VALUE)
      result = DS1808_MAX_POT_VALUE;

   return result | DS1808_POT2_OFFSET;
}

int DS1808LC::getPercentBrightness(uint8_t val1, uint8_t val2) {
   val2 = val2 & (~DS1808_POT2_OFFSET);
   int scaledResistance = val1 + val2;
   int percent;

   if (scaledResistance < DS1808_LOW_VALUE)
      percent = 100;
   else if (scaledResistance > DS1808_HIGH_VALUE)
      percent = 0;
   else
      percent = 100 - (((scaledResistance - DS1808_LOW_VALUE) * 100) / (DS1808_HIGH_VALUE - DS1808_LOW_VALUE));
   return percent;
}

int DS1808LC::getScaledResistance(int dutyPercent) {
   int scaledResistance;

   if (dutyPercent == 0)
      scaledResistance = 2 * DS1808_MAX_POT_VALUE;
   else if (dutyPercent == 100)
      scaledResistance = 0;
   else
      scaledResistance = (((100 - dutyPercent) * (DS1808_HIGH_VALUE - DS1808_LOW_VALUE)) / 100) + DS1808_LOW_VALUE;
   return scaledResistance;
}

}



