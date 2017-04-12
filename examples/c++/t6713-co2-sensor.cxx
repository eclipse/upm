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
#include "t6713.hpp"

#define EDISON_I2C_BUS 1 
#define FT4222_I2C_BUS 0

//! [Interesting]
// Simple example of using ICO2Sensor to determine 
// which sensor is present and return its name.
// ICO2Sensor is then used to get readings from sensor


upm::ICO2Sensor* getCO2Sensor()
{
   upm::ICO2Sensor* cO2Sensor = NULL;
   try {
      cO2Sensor = new upm::T6713(mraa_get_sub_platform_id(FT4222_I2C_BUS));
      return cO2Sensor;
   } catch (std::exception& e) {
      std::cerr << "T6713: " << e.what() << std::endl;      
   }
   return cO2Sensor;   
}

int main ()
{
   upm::ICO2Sensor* cO2Sensor = getCO2Sensor();
   if (cO2Sensor == NULL) {
      std::cout << "CO2 sensor not detected" << std::endl;                        
      return 1;
   }
   std::cout << "CO2 sensor " << cO2Sensor->getModuleName() << " detected" << std::endl;
   while (true) {
      try {
         uint16_t value = cO2Sensor->getPpm();
         std::cout << "CO2 level = " << value << " ppm" << std::endl;
      } catch (std::exception& e) {
         std::cerr << e.what() << std::endl;
      }
      sleep(1);         
   }
   delete cO2Sensor;
   return 0;
}

//! [Interesting]      
