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
#include <iostream>
#include <signal.h>
#include "mpu9250.h"

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

  upm::MPU9250 *sensor = new upm::MPU9250();

  sensor->init();

  while (shouldRun)
    {
      sensor->update();
      
      float x, y, z;
      
      sensor->getAccelerometer(&x, &y, &z);
      cout << "Accelerometer: ";
      cout << "AX: " << x << " AY: " << y << " AZ: " << z << endl;

      sensor->getGyroscope(&x, &y, &z);
      cout << "Gryoscope:     ";
      cout << "GX: " << x << " GY: " << y << " GZ: " << z << endl;
      
      sensor->getMagnetometer(&x, &y, &z);
      cout << "Magnetometer:  ";
      cout << "MX = " << x << " MY = " << y << " MZ = " << z << endl;

      cout << "Temperature:   " << sensor->getTemperature() << endl;
      cout << endl;

      usleep(500000);
    }

//! [Interesting]

  cout << "Exiting..." << endl;
  
  delete sensor;
  
  return 0;
}
