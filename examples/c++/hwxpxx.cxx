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

#include "hwxpxx.hpp"

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

  string defaultDev = "/dev/ttyUSB0";

  // if an argument was specified, use it as the device instead
  if (argc > 1)
    defaultDev = string(argv[1]);

  cout << "Using device " << defaultDev << endl;
  cout << "Initializing..." << endl;

  // Instantiate an HWXPXX instance, using MODBUS slave address 3, and
  // default comm parameters (19200, 8, N, 2)
  upm::HWXPXX *sensor = new upm::HWXPXX(defaultDev, 3);

  // output the Slave ID (manufacturer, model, serno)
  cout << "Slave ID: " << sensor->getSlaveID() << endl;

  // stored temperature and humidity offsets
  cout << "Temperature Offset: " << sensor->getTemperatureOffset()
       << endl;
  cout << "Humidity Offset: " << sensor->getHumidityOffset()
       << endl;

  cout << endl;

  // update and print available values every second
  while (shouldRun)
    {
      // update our values from the sensor
      sensor->update();

      // we show both C and F for temperature
      cout << "Temperature: " << sensor->getTemperature()
           << " C / " << sensor->getTemperature(true) << " F"
           << endl;

      cout << "Humidity: " << sensor->getHumidity()
           << " %" << endl;

      cout << "Slider: " << sensor->getSlider() << " %" << endl;

      cout << "Override Switch Status: " << sensor->getOverrideSwitchStatus()
           <<  endl;

      cout << endl;

      sleep(1);
    }

  cout << "Exiting..." << endl;

  delete sensor;

//! [Interesting]

  return 0;
}
