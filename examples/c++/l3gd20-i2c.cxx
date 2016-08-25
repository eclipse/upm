/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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
#include <iomanip>
#include <math.h>
#include <signal.h>
#include "l3gd20.hpp"

using namespace std;

int shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

float rad2deg(float x)
{
  return x * (180.0 / M_PI);
}


int main(int argc, char **argv)
{
  signal(SIGINT, sig_handler);
//! [Interesting]

  // Instantiate an L3GD20 using default parameters
  upm::L3GD20 *sensor = new upm::L3GD20(L3GD20_DEFAULT_I2C_BUS,
                                        L3GD20_DEFAULT_I2C_ADDR);

  // set some parameters (these are already the defaults, but are
  // provided here as an example)

  // 250 deg/s sensitivity
  sensor->setRange(sensor->FS_250);

  // Set ODR to 95Hz, 25Hz cut-off
  sensor->setODR(sensor->ODR_CUTOFF_95_25);

  // If you already have calibration data, you can specify it here
  //  sensor->loadCalibratedData(-0.0296269637, -0.0080939643, -0.0077121737);

  // now output data every 100 milliseconds
  while (shouldRun)
    {
      float x, y, z;

      sensor->update();

      cout << "Calibrated: " << sensor->getCalibratedStatus() << endl;

      // output is in radians/s
      sensor->getGyroscope(&x, &y, &z);
      cout << fixed << setprecision(1)
           << "Gyroscope x: " << x
           << " y: " << y
           << " z: " << z
           << " radians"
           << endl;

      // same data converted to degrees/s
      cout << "Gyroscope x: " << rad2deg(x)
           << " y: " << rad2deg(y)
           << " z: " << rad2deg(z)
           << " degrees"
           << endl;

      // we show both C and F for temperature
      cout << "Compensation Temperature: " << sensor->getTemperature(false)
           << " C / " << sensor->getTemperature(true) << " F"
           << endl;

      cout << endl;

      usleep(100000);
    }

  // dump the calibration values if we managed to calibrate
  if (sensor->getCalibratedStatus())
    {
      float calX, calY, calZ;
      sensor->getCalibratedData(&calX, &calY, &calZ);

      cout << setprecision(10)
           << "Calibration values x: " << calX
           << " y: " << calY
           << " z: " << calZ
           << endl;
    }

  cout << "Exiting..." << endl;

  delete sensor;

//! [Interesting]
  return 0;
}
