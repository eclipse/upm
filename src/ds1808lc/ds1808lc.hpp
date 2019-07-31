/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#include <string>

#include "mraa/i2c.hpp"
#include "mraa/initio.hpp"

namespace upm
{
/**
 * @brief DS1808LC Lighting Controller
 * @defgroup DS1808LC libupm-ds1808lc
 * @ingroup ds1808lc i2c maxim light ilightcontroller
 */

/**
 * @library ds1808lc
 * @sensor ds1808lc
 * @comname Lighting Controller
 * @altname DS1808LC
 * @type light
 * @man maxim
 * @con i2c
 * @web http://www.maximintegrated.com/en/products/analog/data-converters/digital-potentiometers/DS1808.html
 *
 * @brief API for DS1808 Dual Log Digital Potentiometer as a Light Controller
 *
 * The Maxim Integrated
 * [DS1808](http://www.maximintegrated.com/en/products/analog/data-converters/digital-potentiometers/DS1808.html)
 * Dual Log Digital Potentiometer
 *
 * @snippet ds1808lc.cxx Interesting
 */
#define UPM_THROW(msg) throw std::runtime_error(std::string(__FUNCTION__) + ": " + (msg))

class DS1808LC
{
public:
   DS1808LC(int gpioPower, int i2cBus);
   DS1808LC(std::string initStr);
   ~DS1808LC();

   const char* getModuleName() { return "ds1808lc"; }
   bool isPowered();
   void setPowerOn();
   void setPowerOff();
   int getBrightness();
   void setBrightness(int dutyPercent);

private:
   int getPercentBrightness(uint8_t val1, uint8_t val2);
   uint8_t getPot1Value(int dutyPercent);
   uint8_t getPot2Value(int dutyPercent);
   int getScaledResistance(int dutyPercent);

   mraa::Result status;
   mraa::I2c* i2c;
   mraa::MraaIo mraaIo; 
   int pinPower;
};


}

