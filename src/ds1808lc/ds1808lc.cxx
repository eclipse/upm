#include <iostream>
#include <unistd.h>
#include <string.h>
#include <cmath>
#include "ds1808lc.h"
#include "mraa-utils.h"

#define DS1808_I2C_ADDR 0x28
#define DS1808_POT2_OFFSET 0x40
#define DS1808_MAX_POT_VALUE 0x20
#define DS1808_LOW_VALUE 32 // Lowest pot value that the eye can differentiate from 0
#define DS1808_HIGH_VALUE 51 // Highest pot value that the eye can differentiate from full

// Used to prevent silent failure in mraa_i2c_write()
// Value from I2C_SMBUS_I2C_BLOCK_MAX in i2c driver
#define MAX_I2C_WRITE_SIZE 32



namespace upm {
DS1808LC::DS1808LC(int gpioPower, int i2cBus)
{
   status = MRAA_ERROR_INVALID_RESOURCE;
   mraa_set_log_level(7);
   pinPower = gpioPower;
   i2c = mraa_i2c_init(i2cBus);
   if (i2c == NULL)
   {
      status = MRAA_ERROR_INVALID_RESOURCE;
      printf("DS1808LC: I2C initialisation failed.\n");
      return;
   }
   status = mraa_i2c_address(i2c, DS1808_I2C_ADDR);
}

DS1808LC::~DS1808LC()
{
}


bool DS1808LC::isConfigured()
{
   return status == MRAA_SUCCESS;
}

bool DS1808LC::isPowered()
{
   int level;
   if (MraaUtils::getGpio(pinPower, &level) == MRAA_SUCCESS)
      return static_cast<bool>(level);
   else
      return false;
}

bool DS1808LC::setPowerOn()
{
   if (!isPowered())
   {
      if (MraaUtils::setGpio(pinPower, 1) != MRAA_SUCCESS)
      {
         printf("DS1808LC: Power on failed.\n");
         status = MRAA_ERROR_INVALID_RESOURCE;
         return false;
      }
      if (!setBrightness(0))
         printf("DS1808LC: Failed to set brightness.\n");
   }
   return isConfigured();
}

bool DS1808LC::setPowerOff()
{
   status = MraaUtils::setGpio(pinPower, 0);
   return isConfigured();   
}


bool DS1808LC::getBrightness(int* percent)
{
   uint8_t values[2];

   if (mraa_i2c_read(i2c, values, 2) == 2) {
      *percent = getPercentBrightness(values[0], values[1]);
      status = MRAA_SUCCESS;
   }
   else {
      printf("DS1808LC: Failed to retrieve potentiometer values.\n");
      status = MRAA_ERROR_INVALID_RESOURCE;
   }
   return isConfigured();   
}
   

bool DS1808LC::setBrightness(int dutyPercent)
{
   uint8_t values[2];
   values[0] = getPot1Value(dutyPercent);
   values[1] = getPot2Value(dutyPercent);
   status = mraa_i2c_write(i2c, values, 2);
   return isConfigured();
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



