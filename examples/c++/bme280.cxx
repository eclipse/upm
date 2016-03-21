/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include <unistd.h>
#include <iostream>
#include "bme280.h"

#define FT4222_I2C_BUS 0

int main ()
{
   try {
      upm::BME280* bme280 = new upm::BME280 (mraa_get_sub_platform_id(FT4222_I2C_BUS));
      while (true) {
         int temperature = bme280->getTemperatureCelcius();
         int humidity = bme280->getHumidityRelative();
         int pressure = bme280->getPressurePa();
         std::cout << "Temperature = " << temperature << "C" << std::endl;
         std::cout << "Humidity    = " << humidity << "%" << std::endl;
         std::cout << "Pressure    = " << pressure << "Pa" << std::endl;
         sleep(1);
      }
      delete bme280;
   } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
   }
   return 0;
}

//! [Interesting]
