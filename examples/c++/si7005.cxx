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
#include "si7005.hpp"

#define EDISON_I2C_BUS 1
#define EDISON_GPIO_SI7005_CS 20


int main ()
{
   try {
      upm::SI7005* sensor = new upm::SI7005(EDISON_I2C_BUS, EDISON_GPIO_SI7005_CS);
      while (true) {
         int temperature = sensor->getTemperatureCelsius();
         int humidity = sensor->getHumidityRelative();
         std::cout << "Temperature = " << temperature << "C" << std::endl;
         std::cout << "Humidity    = " << humidity << "%" << std::endl;
         sleep(1);
      }
      delete sensor;
   } catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
   }
   return 0;
}

//! [Interesting]
