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
#include "adxl335.h"

using namespace std;

int shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}


int main ()
{
  signal(SIGINT, sig_handler);

//! [Interesting]
  // Instantiate an ADXL335 accelerometer on analog pins A0, A1, and A2
  upm::ADXL335* accel = new upm::ADXL335(0, 1, 2);

  cout << "Please make sure the sensor is completely still. Sleeping for"
       << " 2 seconds." << endl;
  sleep(2);
  cout << "Calibrating..." << endl;

  accel->calibrate();
  
  while (shouldRun)
    {
      int x, y, z;
      float aX, aY, aZ;

      accel->values(&x, &y, &z);
      cout << "Raw Values: X: " << x << " Y: " << y << " Z: " << z << endl;

      accel->acceleration(&aX, &aY, &aZ);
      cout << "Acceleration: X: " << aX << "g" << endl;
      cout << "Acceleration: Y: " << aY << "g" << endl;
      cout << "Acceleration: Z: " << aZ << "g" << endl;
      cout << endl;

      usleep(200000);
    }
//! [Interesting]

  cout << "Exiting" << endl;

  delete accel;
  return 0;
}
