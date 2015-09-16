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
#include "max44009.h"
#include "si1132.h"

#define EDISON_I2C_BUS 1 
#define FT4222_I2C_BUS 0


int main ()
{
   //! [Interesting]
   // Simple example of using IModuleStatus (inherited by ILightSensor)
   // to determine if which sensor is present and return its name.
   // ILightSensor is then used to get reading from sensor
   upm::ILightSensor* lightSensor = NULL;
   lightSensor = new upm::SI1132(mraa_get_sub_platform_id(FT4222_I2C_BUS));
   if (!lightSensor->isConfigured()) {
      delete lightSensor; 
      lightSensor = NULL; 
   }

   if (lightSensor == NULL)
   {
      lightSensor = new upm::MAX44009(EDISON_I2C_BUS);
      if (!lightSensor->isConfigured()) {
         delete lightSensor; 
         lightSensor = NULL; 
      }
   }

   if (lightSensor != NULL) {
      float value;
      std::cout << "Light sensor " << lightSensor->getModuleName() << " detected" << std::endl;
      // sleep(1);
      if (lightSensor->getValue(&value) == MRAA_SUCCESS)
         std::cout << "Light level = " << value << " lux" << std::endl;
      else
         std::cout << "Failed to get sensor reading" << std::endl;         
      delete lightSensor;
   } else
      std::cout << "Light sensor not detected" << std::endl;                  
   //! [Interesting]      
      
   return 0;
}
