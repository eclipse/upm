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

#include "dfrec.hpp"

using namespace std;

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main()
{
  signal(SIGINT, sig_handler);

//! [Interesting]

  // Instantiate a DFRobot EC sensor on analog pin A0, with a ds18b20
  // temperature sensor connected to UART 0, and a device index (for
  // the ds1820b uart bus) of 0, and an analog reference voltage of
  // 5.0.
  upm::DFREC *sensor = new upm::DFREC(0, 0, 0, 5.0);

  // Every 2 seconds, update and print values
  while (shouldRun)
    {
      sensor->update();

      cout << "EC = "
           << sensor->getEC()
           << " ms/cm"
           << endl;

      cout << "Volts = "
           << sensor->getVolts()
           << ", Temperature = "
           << sensor->getTemperature()
           << " C"
           << endl;

      cout << endl;

      sleep(2);
    }

//! [Interesting]

  cout << "Exiting" << endl;

  delete sensor;
  return 0;
}
