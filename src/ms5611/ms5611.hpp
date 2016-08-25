/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2016 Intel Corporation.
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

#include "upm/iPressureSensor.hpp"
#include "upm/iTemperatureSensor.hpp"
#include "mraa/i2c.hpp"

namespace upm
{

#define MS5611_ADDRESS 0x76
/**
 * @brief ms5611 Barometric Pressure Sensor library
 * @defgroup ms5611 libupm-ms5611
 * @ingroup meas i2c temp pressure ipressuresensor itemperaturesensor
 */

/**
 * @library ms5611
 * @sensor ms5611
 * @comname Meas-Spec MS5611 Barometric Pressure Sensor library
 * @altname MS5611
 * @type temp pressure
 * @man meas
 * @con i2c
 * @if ipressuresensor itemperaturesensor
 *
 * @brief API for MS5611 Barometric Pressure Sensor library
 *
 * Measurement Specialties
 * [MS5611-01BA03](http://www.te.com/usa-en/product-CAT-BLPS0036.html)
 * The MS5611 is a new generation of high resolution altimeter sensors from
 * MEAS Switzerland with SPI and I2C bus interface. This module implements
 * I2C only.
 *
 * @snippet ms5611.cxx Interesting
 */

class MS5611 : public IPressureSensor, ITemperatureSensor
{
public:
   enum OsrMode
   {
       ULTRA_LOW_POWER, LOW_POWER, STANDARD, ULTRA_HIGH_RES, HIGH_RES
   };

   MS5611(int i2cBus = 0, int address = MS5611_ADDRESS);
   ~MS5611();
   const char* getModuleName() { return "ms5611"; }
   void setOverSampling(OsrMode osrMode);
   int getTemperatureCelsius();
   int getPressurePa();

private:
   int promCrc4();
   uint32_t readADC(int adcReg);
   void delayms(int millisecs);
   uint32_t readRawPressure();
   uint32_t readRawTemperature();

   mraa::Result status;
   mraa::I2c* i2c;
   int address;
   uint16_t *prom;
   int osr;
   int samplePeriod;
};


}

