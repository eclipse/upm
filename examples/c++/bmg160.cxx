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
#include "bmg160.hpp"

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

  // Instantiate an BMG160 using default I2C parameters
  upm::BMG160 *sensor = new upm::BMG160();

  // For SPI, bus 0, you would pass -1 as the address, and a valid pin
  // for CS: BMG160(0, -1, 10);

  // now output data every 250 milliseconds
  while (shouldRun)
    {
      float x, y, z;

      sensor->update();

      sensor->getGyroscope(&x, &y, &z);
      cout << "Gyroscope x: " << x
           << " y: " << y
           << " z: " << z
           << " degrees/s"
           << endl;

      // we show both C and F for temperature
      cout << "Compensation Temperature: " << sensor->getTemperature()
           << " C / " << sensor->getTemperature(true) << " F"
           << endl;

      cout << endl;

      usleep(250000);
    }

//! [Interesting]

  cout << "Exiting..." << endl;

  delete sensor;

  return 0;
}
