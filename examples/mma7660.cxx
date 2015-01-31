/*
 * Author: Jon Trulson <jtrulson@ics.com>
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
#include <signal.h>
#include <iostream>
#include "mma7660.h"

using namespace std;

int shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main(int argc, char **argv)
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // Instantiate an MMA7660 on I2C bus 0

  upm::MMA7660 *accel = new upm::MMA7660(MMA7660_I2C_BUS, 
                                         MMA7660_DEFAULT_I2C_ADDR);

  // place device in standby mode so we can write registers
  accel->setModeStandby();

  // enable 64 samples per second
  accel->setSampleRate(upm::MMA7660::AUTOSLEEP_64);
  
  // place device into active mode
  accel->setModeActive();

  while (shouldRun)
    {
      int x, y, z;
      
      accel->getRawValues(&x, &y, &z);
      cout << "Raw values: x = " << x 
           << " y = " << y
           << " z = " << z
           << endl;
      
      float ax, ay, az;
      
      accel->getAcceleration(&ax, &ay, &az);
      cout << "Acceleration: x = " << ax 
           << "g y = " << ay
           << "g z = " << az
           << "g" << endl;
      
      usleep(500000);
    }

//! [Interesting]

  cout << "Exiting..." << endl;

  delete accel;
  return 0;
}
