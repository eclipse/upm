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

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "kx122.h"
#include "upm_utilities.h"

bool shouldRun = true;

void sig_handler(int signo)
{
  if(signo == SIGINT){
    shouldRun = false;
  }
}

int main(int argc, char **argv)
{
  signal(SIGINT,sig_handler);

  //! [Interesting]
  kx122_context sensor = kx122_init(0, -1, 24, 10000);
  if (!sensor)
  {
      printf("kx122_init() failed.\n");
      return 1;
  }

  kx122_sensor_software_reset(sensor);
  kx122_device_init(sensor,KX122_ODR_50,HIGH_RES,KX122_RANGE_2G);

  float x,y,z;
  int wait_time = (kx122_get_sample_period(sensor) * MICRO_S);
  if (wait_time < 0) wait_time = 1000;

  while(shouldRun){
    kx122_get_acceleration_data(sensor,&x,&y,&z);

    printf("%.02f | %.02f | %.02f\n",x,y,z);
    upm_delay_us(wait_time);
  }

  kx122_close(sensor);
  //! [Interesting]

  return 0;
}
