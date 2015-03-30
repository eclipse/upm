#include <iostream>
#include <unistd.h>
#include <string.h>
#include <cmath>
#include "tubelight.h"
#include "mraa-utils.h"

#define TUBELIGHT_I2C_ADDR 0x28

#define TUBELIGHT_POT2_OFFSET 0x40

#define MAX_POT_VALUE 0x20
// Lowest pot value that the eye can differentiate from 0
#define LOW_VALUE 32
// Higest pot value that the eye can differentiate from full
#define HIGH_VALUE 51

// Used to prevent silent failure in mraa_i2c_write()
// Value come from I2C_SMBUS_I2C_BLOCK_MAX in i2c driver
#define MAX_I2C_WRITE_SIZE 32



namespace upm {
TUBELIGHT::TUBELIGHT(int gpioPower, int i2cBus)
{
   status = MRAA_ERROR_INVALID_RESOURCE;
   mraa_set_log_level(7);
   pinPower = gpioPower;
   i2c = mraa_i2c_init(i2cBus);
   if (i2c == NULL)
   {
      status = MRAA_ERROR_INVALID_RESOURCE;
      printf("i2c_init failed\n");
      return;
   }
   mraa_i2c_address(i2c, TUBELIGHT_I2C_ADDR);
   status = MRAA_SUCCESS;

   // There is a bug in the hardware that prevents us from reading the power GPIO pin
   m_isPowered = false;     // Assume it's off
}

TUBELIGHT::~TUBELIGHT()
{
}


bool TUBELIGHT::isOK()
{
   return status == MRAA_SUCCESS;
}

bool TUBELIGHT::getBrightnessRange(int* percentMin, int* percentMax)
{
   *percentMin = 0;
   *percentMax = 100;
   return true;
}

bool TUBELIGHT::isPowered()
{
   // There is a bug where reading the GPIO pin sets it to 0 and powers off the light
   return m_isPowered;
//   int level;
//   if (MraaUtils::getGpio(pinPower, &level) == MRAA_SUCCESS)
//      return level == 1;
//   else
//      return false;
}

bool TUBELIGHT::setPowerOn()
{
   if (!isPowered())
   {
      if (MraaUtils::setGpio(pinPower, 1) != MRAA_SUCCESS)
      {
         printf("setPowerOn failed\n");
         status = MRAA_ERROR_INVALID_RESOURCE;
         return false;
      }
      if (!setBrightness(0))
         printf("setBrightness failed\n");
   }
   m_isPowered = isOK();
   return isOK();
}

bool TUBELIGHT::setPowerOff()
{
   m_isPowered = false;
   return MraaUtils::setGpio(pinPower, 0) == MRAA_SUCCESS;
}


bool TUBELIGHT::getBrightness(int* percent)
{
   uint8_t values[2];

   if (mraa_i2c_read(i2c, values, 2) == 2) {
//      printf("Raw read: Val1 = %02x, Val2 = %02x\n", values[0], values[1]);
      *percent = getPercentBrightness(values[0], values[1]);
//      printf(": %d%%\n", *percent);
      return true;
   }
   else {
      printf(" failed\n");
      return false;
   }
}
   

bool TUBELIGHT::setBrightness(int dutyPercent)
{
   uint8_t values[2];
   values[0] = getPot1Value(dutyPercent);
   values[1] = getPot2Value(dutyPercent);
//   printf("Brightness to %d%%.  Scaled pot = %d, Pot1 = %02x, Pot2 = %02x\n", dutyPercent, values[0] + (values[1] - 0x40), values[0], values[1]);
   mraa_i2c_write(i2c, values, 2);

   bool retval =  isOK();
//   int percent;
//   getBrightness(&percent);
//   printf("Current brightness: %d%%\n", percent);

   return retval;
}


uint8_t TUBELIGHT::getPot1Value(int dutyPercent) {
   uint8_t result = 0;
   int scaledResistance = getScaledResistance(dutyPercent);

   if (scaledResistance > MAX_POT_VALUE)
      result = MAX_POT_VALUE;
   else {
      result = scaledResistance;
   }

   return result;
}

uint8_t TUBELIGHT::getPot2Value(int dutyPercent) {
   uint8_t result = 0;
   int scaledResistance = getScaledResistance(dutyPercent);

   if (scaledResistance <= MAX_POT_VALUE)
      result = 0;
   else {
      result = scaledResistance - MAX_POT_VALUE;
   }

   if (result > MAX_POT_VALUE)
      result = MAX_POT_VALUE;

   return result | TUBELIGHT_POT2_OFFSET;
}

int TUBELIGHT::getPercentBrightness(uint8_t val1, uint8_t val2) {
   val2 = val2 & (~TUBELIGHT_POT2_OFFSET);
   int scaledResistance = val1 + val2;
   int percent;

   if (scaledResistance < LOW_VALUE)
      percent = 100;
   else if (scaledResistance > HIGH_VALUE)
      percent = 0;
   else
      percent = 100 - (((scaledResistance - LOW_VALUE) * 100) / (HIGH_VALUE - LOW_VALUE));
   return percent;
}

int TUBELIGHT::getScaledResistance(int dutyPercent) {
   int scaledResistance;

   if (dutyPercent == 0)
      scaledResistance = 2 * MAX_POT_VALUE;
   else if (dutyPercent == 100)
      scaledResistance = 0;
   else
      scaledResistance = (((100 - dutyPercent) * (HIGH_VALUE - LOW_VALUE)) / 100) + LOW_VALUE;
   return scaledResistance;
}

}



