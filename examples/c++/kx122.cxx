/*
* The MIT License (MIT)
*
* Author: Samuli Rissanen <samuli.rissanen@hotmail.com>
* Copyright (c) 2018 Rohm Semiconductor.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
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
