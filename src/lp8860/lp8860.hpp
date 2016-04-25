/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "upm/iLightController.hpp"
#include "mraa/i2c.hpp"

namespace upm
{
/**
 * @brief lp8860 LED lighting controller library
 * @defgroup lp8860 libupm-lp8860
 * @ingroup ti i2c light ilightcontroller
 */

/**
 * @brief API for LP8860 LED Lighting Controller
 *
 * The Texas Instrumements
 * [LP8860-Q1](http://www.ti.com/product/lp8860-q1)
 * Automotive Catalog Low-EMI, High-Performance 4-Channel LED Driver
 *
 * @library lp8860
 * @sensor lp8860
 * @comname TI LP8860 LED lighting controller
 * @altname LP8860
 * @type light
 * @man ti
 * @con i2c
 * @if ilightcontroller
 *
 */
class LP8860 : public upm::ILightController
{
public:
   LP8860(int gpioPower, int i2cBus);
   ~LP8860();
   const char* getModuleName() { return "lp8860"; }
   bool isPowered();
   void setPowerOn();
   void setPowerOff();
   int getBrightness();
   void setBrightness(int dutyPercent);

private:
   void i2cWriteByte(int reg, int value);
   void i2cWriteBuffer(int reg, uint8_t* buf, int length);
   uint8_t i2cReadByte(uint8_t reg);
   void i2cReadBuffer(int reg, uint8_t* buf, int length);
   void loadEEPROM();
   void allowMaxCurrent();
   bool isAvailable();

   mraa::Result status;
   mraa::I2c* i2c;
   int pinPower;
};


}

