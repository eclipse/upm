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
#include <signal.h>
#include "bno055.hpp"

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

  // Instantiate an BNO055 using default parameters (bus 0, addr
  // 0x28).  The default running mode is NDOF absolute orientation
  // mode.
  upm::BNO055 *sensor = new upm::BNO055();

  // First we need to calibrate....
  cout << "First we need to calibrate.  4 numbers will be output every"
       << endl;
  cout << "second for each sensor.  0 means uncalibrated, and 3 means"
       << endl;
  cout << "fully calibrated."
       << endl;
  cout << "See the UPM documentation on this sensor for instructions on"
       << endl;
  cout << "what actions are required to calibrate."
       << endl;
  cout << endl;

  // do the calibration...
  while (shouldRun && !sensor->isFullyCalibrated())
    {
      int mag, acc, gyr, sys;
      sensor->getCalibrationStatus(&mag, &acc, &gyr, &sys);

      cout << "Magnetometer: " << mag
           << " Accelerometer: " << acc
           << " Gyroscope: " << gyr
           << " System: " << sys
           << endl;

      sleep(1);
    }

  cout << endl;
  cout << "Calibration complete." << endl;
  cout << endl;

  // now output various fusion data every 250 milliseconds
  while (shouldRun)
    {
      float w, x, y, z;

      sensor->update();

      sensor->getEulerAngles(&x, &y, &z);
      cout << "Euler: Heading: " << x
           << " Roll: " << y
           << " Pitch: " << z
           << " degrees"
           << endl;

      sensor->getQuaternions(&w, &x, &y, &z);
      cout << "Quaternion: W: " << w
           << " X: " << x
           << " Y: " << y
           << " Z: " << z
           << endl;

      sensor->getLinearAcceleration(&x, &y, &z);
      cout << "Linear Acceleration: X: " << x
           << " Y: " << y
           << " Z: " << z
           << " m/s^2"
           << endl;

      sensor->getGravityVectors(&x, &y, &z);
      cout << "Gravity Vector: X: " << x
           << " Y: " << y
           << " Z: " << z
           << " m/s^2"
           << endl;

      cout << endl;
      usleep(250000);
    }

//! [Interesting]

  cout << "Exiting..." << endl;

  delete sensor;

  return 0;
}
