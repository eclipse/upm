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

#include "t3311.hpp"

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

  cout << "Initializing..." << endl;

  // Instantiate an T3311 instance, using MODBUS slave address 1, and
  // default comm parameters (9600, 8, N, 2)
  upm::T3311 *sensor = new upm::T3311(defaultDev, 1);

  // output the serial number and firmware revision
  cout << "Serial Number: " << sensor->getSerialNumber() << endl;
  cout << "Firmware Revision: " << sensor->getFirmwareMajor()
       << "." << sensor->getFirmwareMinor() << endl;
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

      // this value depends on the sensor configuration -- by default
      // it is the dew point temperature
      cout << "Computed Value: " << sensor->getComputedValue() << endl;

      // with FW revisions > 2.44, extended computed data is available
      if (sensor->extendedDataAvailable())
        {
          cout << "Dew Point Temperature: " << sensor->getDewPointTemperature()
               << " C / " << sensor->getDewPointTemperature(true) << " F"
               << endl;

          cout << "Absolute Humidity: " << sensor->getAbsoluteHumidity()
               << " g/m3" << endl;

          cout << "Specific Humidity: " << sensor->getSpecificHumidity()
               << " g/kg" << endl;

          cout << "Mixing Ratio: " << sensor->getMixingRatio()
               << " g/kg" << endl;

          cout << "Specific Enthalpy: " << sensor->getSpecificEnthalpy()
               << " kJ/kg" << endl;
        }

      cout << endl;

      sleep(1);
    }

  cout << "Exiting..." << endl;

  delete sensor;

//! [Interesting]

  return 0;
}
