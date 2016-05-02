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

#include "ds18b20.hpp"

using namespace std;

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main(int argc, char **argv)
{
  signal(SIGINT, sig_handler);

//! [Interesting]

  cout << "Initializing..." << endl;

  // Instantiate an DS18B20 instance using the default values (uart 0)
  upm::DS18B20 sensor;

  // locate and setup our devices
  sensor.init();

  cout << "Found " << sensor.devicesFound() << " device(s)" << endl;
  cout << endl;

  // bail if we didn't find anything
  if (!sensor.devicesFound())
    return 1;

  // update and print available values every second
  while (shouldRun)
    {
      // update our values for the first sensor
      sensor.update(0);

      // we show both C and F for temperature for the first sensor
      cout << "Temperature: " << sensor.getTemperature(0)
           << " C / " << sensor.getTemperature(0, true) << " F"
           << endl;

      sleep(1);
    }

  cout << "Exiting..." << endl;

//! [Interesting]

  return 0;
}
