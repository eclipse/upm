/*
* The MIT License (MIT)
*
* Author: Samuli Rissanen <samuli.rissanen@hotmail.com>
* Copyright (c) 2018 Rohm Semiconductor.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <iostream>

#include <signal.h>
#include <unistd.h>

#include "kx122.hpp"
#include "upm_utilities.h"

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT){
    shouldRun = false;
  }
}

int main(int argc, char **argv)
{
  signal(SIGINT,sig_handler);

  //! [Interesting]
  upm::KX122 sensor(0,-1,24);

  sensor.softwareReset();
  sensor.deviceInit(KX122_ODR_50,HIGH_RES,KX122_RANGE_2G);

  float x,y,z;
  int wait_time = sensor.getSamplePeriod() * MICRO_S;
  if (wait_time < 0) wait_time = 1000;

  while(shouldRun){
    sensor.getAccelerationData(&x,&y,&z);

    printf("%.02f | %.02f | %.02f\n",x,y,z);
    upm_delay_us(wait_time);
  }
  //! [Interesting]

  return 0;
}
