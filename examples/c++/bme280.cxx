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

#include "bme280.hpp"

using namespace std;
using namespace upm;

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

  // Instantiate a BME280 instance using default i2c bus and address
  upm::BME280 *sensor = new upm::BME280();

  // For SPI, bus 0, you would pass -1 as the address, and a valid pin for CS:
  // BME280(0, -1, 10);

  while (shouldRun)
    {
      // update our values from the sensor
      sensor->update();

      // we show both C and F for temperature
      cout << "Compensation Temperature: " << sensor->getTemperature()
           << " C / " << sensor->getTemperature(true) << " F"
           << endl;
      cout << "Pressure: " << sensor->getPressure() << " Pa" << endl;
      cout << "Computed Altitude: " << sensor->getAltitude() << " m" << endl;
      cout << "Humidity: " << sensor->getHumidity() << " %RH" << endl;

      cout << endl;

      sleep(1);
    }
//! [Interesting]

  cout << "Exiting..." << endl;

  delete sensor;

  return 0;
}
