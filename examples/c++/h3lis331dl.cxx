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
#include "h3lis331dl.h"

using namespace std;
using namespace upm;

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
  // Instantiate an H3LIS331DL on I2C bus 0

  upm::H3LIS331DL *accel = new upm::H3LIS331DL(H3LIS331DL_I2C_BUS, 
                                               H3LIS331DL_DEFAULT_I2C_ADDR);

  // Initialize the device with default values
  accel->init();

  while (shouldRun)
    {
      int x, y, z;
      float ax, ay, az;

      accel->update();

      accel->getRawXYZ(&x, &y, &z);
      accel->getAcceleration(&ax, &ay, &az);

      cout << "Raw: X = " << x << " Y = " << y << " Z = " << z << endl;
      
      cout << "Acceleration: AX = " << ax << " AY = " << ay << " AZ = " << az
           << endl;

      cout << endl;

      usleep(500000);
    }

//! [Interesting]

  cout << "Exiting..." << endl;

  delete accel;
  return 0;
}
