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

#include "upm/iLightController.h"
#include "mraa/i2c.h"

namespace upm
{
/**
 * @brief TUBELIGHT LED lighting controller library
 * @defgroup TUBELIGHT libupm-tubelight
 */

/**
 * @brief C++ API for LP8860 chip (Ambient Light Sensor)
 *
 * The Texas Instrumements
 * [LP8860-Q1](http://www.ti.com/product/lp8860-q1)
 * Automotive Catalog Low-EMI, High-Performance 4-Channel LED Driver
 *
 * @ingroup lp8860 i2c
 * @snippet lp8860.cxx Interesting
 */
class TUBELIGHT : public upm::ILightController
{
public:
   TUBELIGHT(int gpioPower, int i2cBus);
   ~TUBELIGHT();

protected:
   bool isOK();
   bool isPowered();
   bool setPowerOn();
   bool setPowerOff();
   bool getBrightness(int* percent);
   bool setBrightness(int dutyPercent);
   bool getBrightnessRange(int* percentMin, int* percentMax);

private:
   int getPercentBrightness(uint8_t val1, uint8_t val2);
   uint8_t getPot1Value(int dutyPercent);
   uint8_t getPot2Value(int dutyPercent);
   int getScaledResistance(int dutyPercent);

   mraa_result_t status;
   mraa_i2c_context i2c;
   int pinPower;
   bool m_isPowered;
};


}

