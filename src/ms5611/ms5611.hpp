/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <interfaces/iPressure.hpp>
#include <interfaces/iTemperature.hpp>
#include "mraa/i2c.hpp"

namespace upm
{

#define MS5611_ADDRESS 0x76
/**
 * @brief MS5611 Barometric Pressure Sensor
 * @defgroup ms5611 libupm-ms5611
 * @ingroup meas i2c temp pressure ipressuresensor itemperaturesensor
 */

/**
 * @library ms5611
 * @sensor ms5611
 * @comname Barometric Pressure and Temperature Sensor
 * @altname MS5611
 * @type temp pressure
 * @man meas
 * @con i2c
 * @web http://www.amsys.info/products/ms5611.htm
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

class MS5611 : virtual public iPressure, virtual public iTemperature
{
public:
   enum OsrMode
   {
       ULTRA_LOW_POWER, LOW_POWER, STANDARD, ULTRA_HIGH_RES, HIGH_RES
   };

   MS5611(int i2cBus = 0, int address = MS5611_ADDRESS);
   ~MS5611();
   virtual const char* getModuleName() { return "ms5611"; }
   void setOverSampling(OsrMode osrMode);
   int getTemperatureCelsius();

   /**
    * Returns the temperature in degrees Celsius
    *
    * @return The Temperature in degrees Celsius
    */
   virtual float getTemperature();
   int getPressurePa();

   /**
    * Return the current measured pressure in Pascals (Pa).
    *
    * @return The pressure in Pascals (Pa).
    */
   virtual float getPressure();

private:
   /* Disable implicit copy and assignment operators */
   MS5611(const MS5611&) = delete;
   MS5611 &operator=(const MS5611&) = delete;

   int promCrc4();
   uint32_t readADC(int adcReg);
   void delayms(int millisecs);
   uint32_t readRawPressure();
   uint32_t readRawTemperature();

   mraa::I2c* i2c;
   int address;
   uint16_t *prom;
   int osr;
   int samplePeriod;
};


}

