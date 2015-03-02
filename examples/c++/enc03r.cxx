/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include "enc03r.h"

using namespace std;

bool shouldRun = true;

// analog voltage, usually 3.3 or 5.0
#define CALIBRATION_SAMPLES 1000

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main()
{
  signal(SIGINT, sig_handler);

//! [Interesting]

  // Instantiate a ENC03R on analog pin A0
  upm::ENC03R *gyro = new upm::ENC03R(0);
  
  // The first thing we need to do is calibrate the sensor.
  cout << "Please place the sensor in a stable location, and do not" << endl;
  cout << "move it while calibration takes place." << endl;
  cout << "This may take a couple of minutes." << endl;

  gyro->calibrate(CALIBRATION_SAMPLES);
  cout << "Calibration complete.  Reference value: " 
       << gyro->calibrationValue() << endl;

  // Read the input and print both the raw value and the angular velocity,
  // waiting 0.1 seconds between readings
  while (shouldRun)
    {
      unsigned int val = gyro->value();
      double av = gyro->angularVelocity(val);

      cout << "Raw value: " << val << ", "  
           << "angular velocity: " << av << " deg/s" << endl;
      
      usleep(100000);
    }
//! [Interesting]

  cout << "Exiting" << endl;

  delete gyro;
  return 0;
}
